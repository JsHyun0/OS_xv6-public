# SSU OS 24-2 HW3

> Priority Scheduler Implement & Experiment

## Changed File

- **defs.h**:
  시스템 콜 정의 작성
- **Makefile**:
  사용자 프로그램 ps, HW3 추가
- **param.h**:
  CPU 개수 지정 변경
- **proc.c**:
  scheduler 변경, get(set)_proc_priority 작성
- **proc.h**:
  프로세스 구조체 변경 (priority, execCnt, waitCnt 추가)
- **syscall.c, syscall.h, sysproc.c**
  시스템콜 선언 및 작성
- user.h
  시스템 콜 및 사용자 프로그램 정의
- usys.S
  시스템 콜 선언

## Maked File
- HW3.c
  테스트 코드 작성
- ps.c
  프로세스 상태 확인 함수 및 시스템 콜 작성



