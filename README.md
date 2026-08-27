# Wafer Processing Equipment Simulator

C++로 구현한 **Wafer Processing Equipment 제어 시뮬레이터**입니다.

Command 기반으로 장비 동작을 제어하며, `EquipmentState`, `WaferState`, `Sensor`, `Alarm` 상태를 함께 관리합니다.

Command 실행 전 Validation과 실행 후 PostValidation을 분리하여 장비 상태 전이와 실행 결과를 검증할 수 있도록 설계했습니다.

## Tech Stack

- C++
- STL
- Visual Studio
- Git / GitHub

## 프로젝트에서 해결하고자 한 문제

장비 제어에서는 Command를 단순히 실행하는 것보다 **현재 장비 상태와 운전 조건을 확인한 후 실행하고, 실행 이후 장비가 기대한 상태로 전이되었는지를 검증하는 과정**이 필요하다고 판단했습니다.

이에 따라 Command 실행을 여러 Validation 단계로 분리하고, 실패 원인을 `EventLog`와 `Alarm`으로 구분하여 추적할 수 있도록 설계했습니다.

또한 실행 후 기대 상태에 도달하지 못한 Command는 `FailedCommandQueue`에 저장하여 Retry할 수 있도록 구성했습니다.

## 주요 설계 포인트

- Command 실행 단계를 `Parameter Validation` / `CanExecute` / `Interlock` / `PostValidation`으로 분리
- `EquipmentState`와 `WaferState`를 기반으로 Command 실행 조건과 상태 전이를 관리
- 장비 이상 및 운전 조건 문제는 `Alarm`으로 사용자에게 알리고 `EventLog`를 통해 실행 결과를 기록
- 실패한 Command는 `FailedCommandQueue`에 저장하고 기존 Command 실행 흐름을 재사용하여 Retry
- Enum별 문자열 변환 로직을 `ToString` 함수로 분리하여 출력 코드의 중복 제거

## Command 실행 흐름

Command가 `CommandQueue`에 등록되면 `RunCommand()`를 통해 순차적으로 실행됩니다.

```text
CommandQueue
    ↓
RunCommand()
    ↓
Parameter Validation
    ↓
CanExecute
    ↓
InterlockValidation
    ↓
Execute
    ↓
PostValidation
    ↓
실행 결과에 따른 EventLog 기록
```

### 사전 검증 실패

```text
Parameter Validation / CanExecute / InterlockValidation
                     ↓
                EventLog 기록
                     ↓
             CommandQueue에서 제거
```

### PostValidation 실패

```text
Execute
   ↓
PostValidation 실패
   ↓
FailedCommandQueue 저장
   ↓
EventLog 기록
```

### 정상 실행

```text
Execute
   ↓
PostValidation 성공
   ↓
Success EventLog 기록
```

## Validation 구조

Command 실행 전 입력값과 장비 상태를 단계별로 검증하고, Execute 이후에는 실제 상태가 기대 상태와 일치하는지 확인합니다.

### Parameter Validation

Command 실행에 필요한 입력 파라미터가 유효한지 확인합니다.

현재 다음 Command의 입력값을 검증합니다.

- `SetRecipe`
- `LoadWafer`

별도의 입력 파라미터가 없는 Command는 검증을 통과시킵니다.

### CanExecute

현재 `EquipmentState`에서 해당 Command를 실행할 수 있는지 확인합니다.

장비 상태가 Command의 실행 조건을 만족하지 않는 경우 Command를 실행하지 않습니다.

일부 장비 이상 또는 운전 조건 문제는 `Alarm`을 발생시키고 Command 실행을 중단합니다.

### InterlockValidation

Command 실행 직전 `Sensor`와 `Alarm` 상태를 확인하여 실제 장비 운전 조건을 검증합니다.

운전 조건을 만족하지 않는 경우 Command 실행을 중단하고, 필요한 경우 `Alarm`을 발생시킵니다.

### PostValidation

Execute 이후 `EquipmentState`, `WaferState`, `Sensor`, `Alarm` 상태를 확인하여 Command가 기대한 상태로 전이되었는지 검증합니다.

PostValidation에 실패한 Command는 `FailedCommandQueue`에 저장하고 실패 원인을 `EventLog`에 기록합니다.

## EquipmentState

`EquipmentState`는 장비의 현재 운전 상태를 표현하며, Command 실행 가능 여부와 상태 전이를 관리하는 기준으로 사용합니다.

### 주요 State 전이

```text
IDLE
  ↓ Initialize
INITIALIZING
  ↓ CompleteInitialization
READY
  ↓ LoadWafer
Loading
  ↓ Start
RUNNING
  ↓ Complete
READY
```

### Error Recovery

```text
RUNNING
   ↓ RaiseError
ERROR
   ↓ Reset
READY
```

## WaferState

`WaferState`는 장비 내부에서 처리되는 Wafer의 현재 상태를 표현합니다.

Wafer의 상태는 Command 실행 조건과 PostValidation에서 사용되며, 장비의 현재 상태와 함께 Command 실행 가능 여부 및 실행 결과를 판단하는 기준이 됩니다.

### 주요 State 전이

```text
EMPTY
  ↓ Load
LOADED
  ↓ StartProcessing
PROCESSING
  ↓ CompleteProcess
COMPLETED
  ↓ ResetProcess
EMPTY
```

`EquipmentState`와 `WaferState`는 서로 다른 상태를 관리하지만, Command 실행 과정에서는 두 상태를 함께 확인하여 장비 상태와 Wafer 처리 상태가 일치하는지 검증합니다.

## Alarm / EventLog

Command 실행 중 발생하는 문제를 `Alarm`과 `EventLog`의 역할에 따라 구분하여 처리합니다.

| 구분 | 역할 |
|---|---|
| `Alarm` | 사용자에게 장비 이상 및 운전 조건 문제를 알림 |
| `EventLog` | Command 실행 결과와 실패 원인을 기록 |
| `AlarmHistory` | 발생한 Alarm 이력을 저장 |

### Alarm

`Alarm`은 장비 이상이나 운전 조건 문제를 사용자에게 알리기 위한 용도로 사용합니다.

Alarm이 발생하면 `AlarmManager`에서 현재 Alarm을 관리하고, 발생한 Alarm은 `AlarmHistory`에 저장합니다.

### EventLog

`EventLog`는 Command 실행 결과와 실패 원인을 기록하여 Command 실행 흐름을 추적할 수 있도록 합니다.

Command가 실패한 경우 실패한 Validation 단계에 따라 다음과 같은 결과를 기록합니다.

- `ParameterValidationFailed`
- `CanExecuteFailed`
- `InterlockFailed`
- `PostValidationFailed`

### Alarm과 EventLog의 관계

`Alarm`은 사용자에게 현재 장비의 이상 상황을 알리는 역할을 담당하고, `EventLog`는 어떤 Command가 어떤 단계에서 실패했는지를 기록하는 역할을 담당합니다.

따라서 장비 이상이나 운전 조건 문제로 Alarm이 발생하는 경우에도 Command 실행 결과는 EventLog를 통해 별도로 기록합니다.

## Retry

Execute 이후 PostValidation에 실패한 Command는 `FailedCommandQueue`에 저장하여 재실행할 수 있도록 구성합니다.

### Retry 처리 흐름

```text
FailedCommandQueue
        ↓
RetryCount 확인
        ↓
RetryCount < 최대 횟수?
   ┌───────────────┐
   │               │
  Yes              No
   ↓               ↓
RetryCount 증가   Retry 중단
   ↓
CommandQueue 재등록
   ↓
RunCommand()
```

Retry 대상 Command는 별도의 실행 로직을 사용하지 않고 기존 `RunCommand()`를 통해 동일한 Validation 및 Execute Flow를 재사용합니다.

## 주요 문제 해결 과정

### 1. Command 실행 검증과 Alarm 처리의 책임 분리

**문제**

초기 구현에서는 Command 실행 함수 내부에서 실행 조건 검사와 Alarm 처리가 함께 이루어졌습니다.

**개선**

이를 `Parameter Validation` / `CanExecute` / `InterlockValidation`으로 분리하고, 실제 Command 실행은 `Execute`에서 담당하도록 구조를 정리했습니다.

**결과**

Command 실행 조건을 단계별로 확인하고, 실패 원인을 `EventLog`로 구분할 수 있도록 개선했습니다.

### 2. Alarm과 EventLog의 중복 출력 문제

**문제**

초기 구현에서는 실패 Command를 출력하는 과정에서 이미 발생한 Alarm 정보가 다시 출력되어 동일한 Alarm이 반복되는 문제가 있었습니다.

**개선**

Alarm은 발생 시점에 사용자에게 즉시 출력하고, `EventLog`와 `FailedCommandQueue`는 각각 기록과 상태 확인의 역할을 담당하도록 출력 책임을 분리했습니다.

**결과**

동일한 Alarm이 여러 경로에서 반복 출력되는 문제를 줄였습니다.

### 3. Reset 이후 장비 상태 복구 문제

**문제**

Error 상태에서 Reset 이후 `EquipmentState`만 READY로 변경하면 `Wafer` / `Sensor` / `Alarm` 상태와 불일치가 발생할 수 있었습니다.

**개선**

Reset 시 `EquipmentState`와 함께 `Wafer` / `Sensor` / `Alarm` 상태를 함께 복구하도록 수정했습니다.

**결과**

Reset 이후 기존 Command Flow를 정상적으로 이어갈 수 있도록 개선했습니다.

### 4. Retry 실행 구조 개선

**문제**

Retry를 별도의 실행 경로로 처리할 경우 일반 Command 실행 로직과 중복이 발생할 수 있었습니다.

**개선**

Retry 대상 Command를 `CommandQueue`에 다시 등록하고 기존 `RunCommand()`를 통해 실행하도록 구성했습니다.

**결과**

Retry에서도 기존 Validation / Execute Flow를 재사용할 수 있도록 개선했습니다.

## 테스트 및 검증

| 테스트 | 검증 내용 | 결과 |
|---|---|---|
| Normal Flow | 전체 Command 실행 및 정상 State 전이 | ✅ Pass |
| Error Recovery | `RaiseError` → `Reset` 이후 정상 Flow 복구 | ✅ Pass |
| Validation Failed | 실행 조건을 만족하지 않는 Command 처리 | ✅ Pass |
| Retry Failed | `FailedCommandQueue`의 Command 재실행 | ✅ Pass |

모든 테스트에서 예상한 실행 결과와 실제 실행 결과가 일치하는 것을 확인했습니다.

## 기술적으로 배운 점

### 상태 기반 제어의 중요성

Command 실행 자체보다 현재 장비 상태와 Wafer / Sensor 상태의 일관성을 유지하는 것이 중요하다는 점을 확인했습니다.

### Validation 책임 분리

실행 가능 여부와 실행 결과 검증을 분리하면 실패 원인을 명확하게 구분하고 각 단계의 책임을 관리하기 쉬워진다는 것을 경험했습니다.

### 실패 처리 설계

`Alarm`, `EventLog`, `FailedCommandQueue`를 각각 다른 목적에 사용하면서 사용자에게 알리는 정보와 시스템이 추적해야 하는 정보를 분리하는 설계의 필요성을 확인했습니다.

### 기존 실행 Flow 재사용

Retry를 별도로 구현하지 않고 기존 `RunCommand()`를 재사용하면 중복된 실행 로직을 만들지 않고 동일한 Validation 정책을 유지할 수 있다는 점을 경험했습니다.

## 향후 개선 방향

- Command 종류 증가에 대비한 Command Dispatch 구조 확장 검토
- 실패 유형별 Retry 가능 여부를 구분하는 Retry 정책 세분화
- Alarm / EventLog에 발생 시간 및 상세 정보 추가
- 테스트 코드를 별도의 테스트 프레임워크 기반으로 분리
- 장비 상태 및 Command Flow를 확인할 수 있는 UI 추가
- 장비와 상위 시스템 간 통신 기능 추가