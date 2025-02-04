# CustomPawn 프로젝트

이 프로젝트는 언리얼 엔진에서 Enhanced Input 시스템을 활용하여 Pawn 클래스를 기반으로 한 3D 캐릭터를 구현한 예제입니다.  
Pawn은 직접 작성한 이동, 회전, 중력, 점프 로직을 포함하며, CapsuleComponent를 루트로 하고 SkeletalMeshComponent, SpringArmComponent, CameraComponent를 포함하고 있습니다.

## 개요

- **직접 이동/회전 구현**:  
  - 카메라의 Forward 및 Right 벡터를 기준으로 이동
  - 마우스 입력을 통해 Pawn의 회전(Yaw) 제어

- **Enhanced Input 시스템 사용**:  
  - 이동 입력은 **IA_MoveForward**와 **IA_MoveRight** 액션으로 분리하여 처리
  - **IA_LookAction**: 마우스 이동으로 Pawn 회전 구현  
  - **IA_Jump**: 점프 입력 처리

- **중력 및 점프 구현**:  
  - 직접 중력 가속도를 계산하여 Pawn에 적용  
  - 캡슐 하단의 라인 트레이스를 이용해 지면 감지  
  - 에어 컨트롤(공중 이동 제한)을 지원

## 기능

- **컴포넌트 구성**
  - **UCapsuleComponent**: Pawn의 루트 충돌 컴포넌트 (물리 시뮬레이션 비활성화)
  - **USkeletalMeshComponent**: 캐릭터 메쉬 (물리 시뮬레이션 비활성화)
  - **USpringArmComponent**: 카메라의 위치와 회전을 조정
  - **UCameraComponent**: 3인칭 카메라

- **입력 및 이동/회전 로직**
  - **MoveForward / MoveRight**: 카메라 기준의 Forward/Right 벡터에 따라 이동
  - **Look**: 마우스 입력에 따른 Pawn 회전 (Yaw) 제어
  - **JumpPressed**: 점프 입력 시 점프 속도(FallingVelocity)를 설정하여 점프 구현

- **중력 및 지면 감지**
  - **ApplyGravity**: Pawn이 공중에 있을 경우 GravityAcceleration을 반영하여 FallingVelocity를 계산
  - **UpdateGroundedState**: 캡슐 하단에서 라인 트레이스를 수행하여 지면과의 접촉 여부(bIsGrounded) 확인


## 코드 구조

- **ACustomPawn 클래스**  
  - **컴포넌트 구성**:  
    - **CapsuleComponent**: 루트 충돌 컴포넌트  
    - **MeshComponent**: 캐릭터 메쉬 (물리 시뮬레이션 비활성화)  
    - **SpringArmComponent**: 카메라의 위치 조정  
    - **CameraComponent**: 3인칭 카메라  
  - **입력 처리 함수**:  
    - `MoveForward(const FInputActionValue& Value)`: 전후 이동 (카메라의 Forward 벡터 기준)  
    - `MoveRight(const FInputActionValue& Value)`: 좌우 이동 (카메라의 Right 벡터 기준)  
    - `Look(const FInputActionValue& Value)`: 마우스 입력에 따른 Pawn 회전  
    - `JumpPressed()`: 점프 입력 처리
  - **중력 및 지면 감지**:  
    - `ApplyGravity(float DeltaTime)`: 중력 가속도를 Pawn에 적용  
    - `UpdateGroundedState(float DeltaTime)`: 캡슐 하단의 라인 트레이스를 통해 지면 접촉 여부 확인

## 참고 사항

- **오디오 타임아웃 경고**  
  언리얼 엔진 실행 종료(PIE 종료) 시, AudioMixer 관련 타임아웃 경고 메시지가 발생합니다.
  재생 및 중지일 때 좀 느리게 종료되거나 느리게 시작 됩니다. 해결 방법을 잘 모르겠습니다.






