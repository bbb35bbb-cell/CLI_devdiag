# devdiag

Linux PC 진단용 C++17 CLI 도구입니다. 시스템을 변경하지 않고 Linux 표준
인터페이스에서 OS, CPU, 메모리, 파일시스템, 네트워크 정보를 읽습니다.

현재 구현된 명령은 `system`, `cpu`, `memory`, `disk`, `network`, `collect`,
`help`입니다. 초기 계획에 있던 `process`와 `service`는 아직 구현하지
않았으므로 지원 명령으로 취급하지 않습니다.

## Build

Requirements:

- CMake 3.16 or later
- C++17 compatible compiler
- Linux (`/proc`, `getifaddrs`, `uname` 사용)

Build the project from the project directory:

```bash
./build.sh build
```

Run the built program:

```bash
./build/devdiag system
```

To rebuild after changing the source code, run the build command again:

```bash
./build.sh build
```

To remove the generated build files:

```bash
./build.sh clean
```

`BUILD_TYPE=Debug`, `JOBS=2` 환경변수로 빌드 타입과 병렬도를 조정할 수
있습니다. `PREFIX="$PWD/install" ./build.sh install`로 설치할 수 있습니다.

```text
devdiag system
devdiag cpu
devdiag memory
devdiag disk
devdiag network
devdiag collect
```

| Command           | 역할               | 주요 Linux 정보                                    |
| ----------------- | ---------------- | ---------------------------------------------- |
| `devdiag system`  | 시스템 기본 정보 조회     | `/etc/os-release`, `uname()`, `/proc/uptime`   |
| `devdiag cpu`     | CPU 정보 및 사용률 조회  | `/proc/cpuinfo`, `/proc/stat`, `/proc/loadavg` |
| `devdiag memory`  | 메모리 사용 상태 조회     | `/proc/meminfo`                                |
| `devdiag disk`    | 디스크/파일시스템 사용량 조회 | `statvfs()`, `/proc/mounts`                    |
| `devdiag network` | 네트워크 인터페이스 상태 조회 | `getifaddrs()`, `/sys/class/net`               |
| `devdiag collect` | 위 진단 정보를 한번에 수집  | 각 모듈 재사용                                       |

`disk`는 `--path <path>`, `network`는 `--interface <name>`을 지원합니다.
자세한 옵션은 `./build/devdiag help`에서 확인할 수 있습니다.


```bash
devdiag system
      ↓
SystemCommand
      ↓
SystemInfoProvider
      ↓
Linux 정보 조회
      ↓
SystemInfo 객체 생성
      ↓
Console 출력
```

## 아키텍처

실행 계층은 `main → parser → command → provider → Linux source`입니다.
Provider는 데이터를 반환하고 Command가 표시 형식과 종료 코드를 담당합니다.
상세 설계는 [ARCHITECTURE.md](ARCHITECTURE.md)를 참고하세요.

```text
main
 ├─ parser                 명령과 옵션 검증
 └─ command                사용 사례 조합과 콘솔 출력
     ├─ SystemCommand
     └─ NetworkCommand
         └─ provider        Linux 데이터 수집
             ├─ SystemInfoProvider
             ├─ CpuInfoProvider
             ├─ MemoryInfoProvider
             ├─ DiskInfoProvider
             └─ NetworkInfoProvider
```

## 제한 및 종료 코드

- `0`: 성공
- `1`: 진단 실행 실패
- `2`: 잘못된 명령 또는 옵션
- Linux 전용이며 `/proc`, `/etc/os-release`, 네트워크 인터페이스가 없는
  환경에서는 일부 값이 0 또는 빈 문자열일 수 있습니다.
- 기본 동작은 읽기 전용이며 root 권한을 요구하지 않습니다.
- `--json`과 `--verbose`는 parser에 예약되어 있으나 현재 출력 형식에는
  아직 반영되지 않았습니다.


## 개발 명령

```bash
./build.sh build
./build.sh test
./build.sh install
./build.sh clean
```

`BUILD_TYPE=Debug`, `JOBS=2`, `PREFIX="$PWD/install"` 환경변수로 빌드 타입,
병렬도, 설치 경로를 지정할 수 있습니다.
