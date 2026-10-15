# devdiag software architecture

## 목표

`devdiag`는 Linux의 표준 인터페이스(`/proc`, `/etc/os-release`, `uname`,
`getifaddrs`, `std::filesystem`)에서 진단 데이터를 읽어 사람이 읽기 쉬운
CLI 출력으로 제공한다. 네트워크나 시스템 설정을 변경하지 않는 읽기 전용
도구를 원칙으로 한다.

## 계층

```text
main
 ├─ parser                 명령/옵션 검증
 └─ command                사용 사례 조합 및 출력
     ├─ SystemCommand
     └─ NetworkCommand
         └─ provider        Linux별 데이터 수집
             ├─ SystemInfoProvider
             ├─ CpuInfoProvider
             ├─ MemoryInfoProvider
             ├─ DiskInfoProvider
             └─ NetworkInfoProvider
```

- `include/*`는 공개 계약(데이터 구조, provider, command)을 둔다.
- `src/*`는 구현을 둔다. Linux 파일시스템 접근은 provider에만 둔다.
- command는 수집 데이터와 표시 형식을 연결하며, provider는 `std::cout`에
  직접 출력하지 않는다.
- 오류가 있는 경우 빈 값/0을 안전한 기본값으로 반환하고 command는 실패를
  사용자에게 알린다. 향후에는 `Result<T, DiagnosticError>`로 확장한다.

## 실행 흐름

1. `main`이 argv를 `CommandLineOptions`로 검증한다.
2. command가 필요한 provider만 호출한다.
3. provider가 Linux 표준 인터페이스를 읽고 값 단위(KiB, byte, percent)를
   명확히 유지한다.
4. command가 현재는 텍스트로 출력한다. `--json`은 JSON renderer를 추가할
   때 사용하는 확장 지점으로 예약되어 있다.

## 확장 규칙

새 진단 영역은 `include/<area>/<area>_info.hpp`,
`src/<area>/<area>_info.cpp`, command 출력 및 README 명령 표를 함께
추가한다. 수집 코드와 출력 코드를 섞지 말고, root 권한을 요구하는 기능은
기본 명령에 넣지 않는다.
