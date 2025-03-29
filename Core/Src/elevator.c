#include "elevator.h"

// 전역 변수 선언
uint8_t internal_requests[3] = {0}; // 내부 요청 배열 (최대 3개 층: B1, F1, F2)
uint8_t up_requests[3] = {0};       // 위로 가는 외부 요청 배열 (최대 3개)
uint8_t down_requests[3] = {0};     // 아래로 가는 외부 요청 배열 (최대 3개)
uint8_t current_floor=set_currentstate;       // 현재 층 (정적 변수로 외부 접근 제한)
uint8_t start_floor=current_floor;
uint8_t next_floor = current_floor;          // 다음 목표 층 (정적 변수로 외부 접근 제한)
ElevatorState state = IDLE;         // 엘리베이터 상태 (IDLE: 정지, UP: 상승, DOWN: 하강)

// 층을 문자열로 변환하는 헬퍼 함수
// 매개변수: floor - 층 번호 (B1, F1, F2)
// 반환값: 해당 층의 문자열 표현
const char *floor_to_str(uint8_t floor) {
    switch (floor) {
        case B1: return "B1";   // 지하 1층
        case F1: return "F1";   // 1층
        case F2: return "F2";   // 2층
        default: return "Unknown"; // 알 수 없는 층
    }
}

// 상태를 문자열로 변환하는 헬퍼 함수
// 매개변수: s - 엘리베이터 상태 (IDLE, UP, DOWN)
// 반환값: 해당 상태의 문자열 표현
const char *state_to_str(ElevatorState s) {
    switch (s) {
        case IDLE: return "IDLE";   // 정지 상태
        case UP: return "UP";       // 상승 상태
        case DOWN: return "DOWN";   // 하강 상태
        default: return "Unknown";  // 알 수 없는 상태
    }
}

// 내부 요청 배열 포인터 반환
// 반환값: internal_requests 배열의 포인터
uint8_t *get_internal_requests(void) {
    return internal_requests;
}

// 위로 가는 요청 배열 포인터 반환
// 반환값: up_requests 배열의 포인터
uint8_t *get_up_requests(void) {
    return up_requests;
}

// 아래로 가는 요청 배열 포인터 반환
// 반환값: down_requests 배열의 포인터
uint8_t *get_down_requests(void) {
    return down_requests;
}

// 현재 층을 센서 데이터로 업데이트
// 센서 데이터를 기반으로 current_floor 값을 설정
void set_currentfloor(void) {
    uint8_t prev_floor = current_floor; // 이전 층 저장
    if (Get_3cm_photo_data() == 1)
    {
        // 3cm 센서 감지 시 B1
        current_floor = B1;
    }
    else if (Get_9cm_photo_data() == 1)
    {
      // 9cm 센서 감지 시 F1
        current_floor = F1;
    }
    else if (Get_15cm_photo_data() == 1)
    {
      // 15cm 센서 감지 시 F2
        current_floor = F2;
    }
    // 비정상적인 층 변경 감지
    if (abs(current_floor - prev_floor) > 1)
    {
        printf("Warning: Abnormal floor change detected! Prev: %s, Current: %s\n",
               floor_to_str(prev_floor), floor_to_str(current_floor));
    }
}

// 엘리베이터 상태 설정
// 현재 층과 다음 층을 비교하여 상태(IDLE, UP, DOWN)를 결정
void set_state(void) {
    if (current_floor == next_floor)
    {
        state = IDLE;
        printf("State changed to IDLE after request processing \n");
    }
    else if (current_floor < next_floor)
    {
        state = UP;
        printf("State changed to UP after request processing\n");
    }
    else if (current_floor > next_floor)
    {
        state = DOWN;
        printf("State changed to DOWN after request processing\n");
    }
}
// 요청 배열에 요청이 있는지 확인
// 매개변수: requests - 확인할 요청 배열
// 반환값: 1 (요청 있음), 0 (요청 없음)
uint8_t check_request(uint8_t *requests) {
    for (int i = 0; i < 3; i++) {
        if (requests[i] != 0) { // 요청이 0이 아니면 (즉, 층 요청이 존재)
            return 1;
        }
    }
    return 0;
}

// 문 열고 닫기 동작 시뮬레이션
// 1초 대기 후 현재 층에서 문 열고 닫는 동작 출력
void door_open_close(void) {
    if(current_floor==F2)
    {
    	if(0)
    	{
    		Servo_Rotate(30, 20);
    		HAL_Delay(10);
    		Servo_Rotate(80,20);
    	}
    	else
    	{
    		return;
    	}
    }

	Servo_Rotate(30, 20);
	HAL_Delay(10);
	Servo_Rotate(80,20);

	HAL_Delay(1000); // 1초 대기 (문 열고 닫는 시간)

    printf("Door open/close at floor %s\n", floor_to_str(current_floor));
}

// 엘리베이터를 한 층 위로 이동
// 스텝 모터를 사용해 상승 동작 수행
void operate_elevator_up(void) {
    if (current_floor < F2) { // 최상층(F2) 미만이면 이동 가능
        printf("Before UP - Floor: %s, Next: %s\n", floor_to_str(current_floor), floor_to_str(next_floor));
        rotateDegrees(760, DIR_CW); // 시계 방향으로 90도 회전 (상승)
        current_floor++; // 현재 층 증가
        set_currentstate(); // 센서로 현재 층 갱신
        printf("Moved UP to floor %s\n", floor_to_str(current_floor));
    }
    request_process(); // 이동 완료 후 다음 요청 확인
}

// 엘리베이터를 한 층 아래로 이동
// 스텝 모터를 사용해 하강 동작 수행
void operate_elevator_down(void) {
    if (current_floor > B1) { // 최하층(B1) 초과면 이동 가능
        printf("Before DOWN - Floor: %s, Next: %s\n", floor_to_str(current_floor), floor_to_str(next_floor));
        rotateDegrees(760, DIR_CCW); // 반시계 방향으로 90도 회전 (하강)
        current_floor--; // 현재 층 감소
        set_currentstate(); // 센서로 현재 층 갱신
        printf("Moved DOWN to floor %s\n", floor_to_str(current_floor));
    }
    request_process(); // 이동 완료 후 다음 요청 확인
}

// 특정 층 요청의 인덱스 찾기
// 매개변수: requests - 요청 배열, floor - 찾을 층 번호
// 반환값: 요청 인덱스 (없으면 -1)
int find_request(uint8_t *requests, uint8_t floor) {
    for (int i = 0; i < 3; i++) {
        if (requests[i] == floor) {
            return i; // 해당 층의 인덱스 반환
        }
    }
    return -1; // 요청이 없으면 -1 반환
}

// 외부 인터럽트로 요청 추가
// 매개변수: requests - 추가할 요청 배열, operator - 추가할 층 번호
void push_requests(uint8_t *requests, uint8_t operator) {
    uint8_t count = 0;
    for (int i = 0; i < 3; i++) {
        if (requests[i] != 0) {
            count++; // 현재 요청 개수 계산
        }
        if (requests[i] == operator) {
            return; // 중복 요청이면 추가하지 않음
        }
    }
    if (count < 3) { // 배열이 꽉 차지 않았으면
        requests[count] = operator; // 빈 자리에 요청 추가
        char msg[64];
        sprintf(msg, "Pushed request %d to %s\n", operator,
                (requests == internal_requests) ? "internal" :
                (requests == up_requests) ? "up" : "down");
        HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);

        sprintf(msg, "After push - [%d,%d,%d]\n", requests[0], requests[1], requests[2]);
        HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
    }
}

// 요청 제거 (처리 완료 또는 버튼 토글)
// 매개변수: requests - 요청 배열, floor - 제거할 층, is_processed - 처리 완료 여부 (1: 완료, 0: 토글)
void remove_request(uint8_t *requests, uint8_t floor, uint8_t is_processed)
{
    int index = find_request(requests, floor);
    if (index != -1) { // 요청이 존재하면
        for (int i = index; i < 2; i++)
        {
            requests[i] = requests[i + 1]; // 뒤의 요소를 앞으로 이동
        }
        requests[2] = 0; // 마지막 요소 초기화
        char msg[64];
        if (is_processed)
        {
            sprintf(msg, "Popped request %d from %s\n", floor,
                    (requests == internal_requests) ? "internal" :
                    (requests == up_requests) ? "up" : "down");
        }
        else
        {
            sprintf(msg, "Request %d removed from %s\n", floor,
                    (requests == internal_requests) ? "internal_requests" :
                    (requests == up_requests) ? "up_requests" : "down_requests");
        }
        HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);

        sprintf(msg, "After remove - [%d,%d,%d]\n", requests[0], requests[1], requests[2]);
        HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
    }
}
//정지상태에서 요청 처리
//요청은 처리를 하나 끝내고 다시 요청을 확인
void request_process_IDLE(void) {
    if(!check_request(internal_requests) && !check_request(up_requests) && !check_request(down_requests))
    {
      printf("NO REQUSESTS\r\n");
      state=IDLE;
    }
    //현재 층 설정
    set_currentfloor();

    //**** 같은 층관 관련된 요청 우선 처리*****//
    // 내부 요청 우선 처리
    // 같은 층 요청 처리여부 확인하는 flag
    uint8_t same_floor_request_complete_flag = 0;
    if (check_request(internal_requests)) {

        for(uint8_t i = 0 ; i < 3 ; i++)
        {
	   // 내부 요청에서 같은 층 요청이 있다면
           // 현재 1층 (F1)-> 2
           // internal_requests[i]에 2가 있다면 같은 층 요청 있다고 판단
	   if(internal_requests[i] == current_floor)
	   {
	      //현재 층 및 처리할 요청 출력
	       printf("current floor : %s ,internal requests processing begin \r\n",floor_to_str(current_floor));
	       //다음 층으로 현재 층으로 설정해서 상태 결정
	       next_floor=set_currentfloor();

	       //아무것도 실행하지 않고 요청 처리
	       remove_request(internal_requests, current_floor, 1);
	       //같은 층과 관련된 외부요청을 전부 처리상태로 변경(삭제)
	       if(find_request(up_requests, current_floor) == 1)
	       {
		    remove_request(up_requests, current_floor, 1);
	       }
	       if(find_request(down_requests, current_floor) == 1)
	       {
		    remove_request(down_requests, current_floor, 1);
	       }
	       same_floor_request_complete_flag=1;
	       printf("current floor : %s internal_requests to process complete !! \r\n ",floor_to_str(current_floor), internal_requests[i]);
	       break;
	   }
	}
        if(!same_floor_request_complete_flag)
        {
          printf(" No current floor : %s internal requests \n", floor_to_str(current_floor));
        }
    }
    //내부 요청 없고 외부요청이 있다면
    else if(!same_floor_request_complete_flag && (check_request(up_requests) || (check_request(down_requests))))
    {
	//외부 요청이 같은 층이라면
	if (find_request(up_requests, current_floor) || find_request(down_requests, current_floor))
	{
	  printf("current floor : %s , same floor external requests processing begin",floor_to_str(current_floor));
	  //다음 층을 현재 층으로 설정
	  next_floor=set_currentfloor();
	  door_open_close();

	  if (find_request(up_requests, current_floor))
	  {
	      remove_request(up_requests, current_floor, 1);
	  }
	  if (find_request(down_requests, current_floor))
	  {
	      remove_request(down_requests, current_floor, 1);
	  }
	  same_floor_request_complete_flag=1;
	}
	if(!same_floor_request_complete_flag)
	{
	  printf(" No current floor : %s internal requests \n", floor_to_str(current_floor));
	}
    }


    //****같은 층 요청 처리 다하고 다른 층 요청 확인 후 요청 처리 시작****//
    //같은 층 요청이 존재하지 않는디 확인
    if (!same_floor_request_complete_flag || !(find_request(internal_requests, current_floor) ||
                                     find_request(up_requests, current_floor) ||
                                     find_request(down_requests, current_floor)))
    {
      //같은 층 요청 전부 처리했는데  다른 층 요청도 없다면 state IDLE로 설정
      if(!check_request(internal_requests) && !check_request(up_requests) && !check_request(down_requests))
      {
           printf("NO REQUSESTS\r\n");
           state=IDLE;
      }
      else
      {
	 switch(current_floor)
	 {
	   case (B1):
	       break;
	   case (F1):
	       break;
	   case (F2):
	       break;
	 }
      }
    }





}
//이동 상태에서 요청 처리
void requests_process_Moving(void)
{

}

// 엘리베이터 동작 관리
void elevator_process(void)
{

    if (state == IDLE)
    {
       request_process_IDLE();
    }
    else if(state == UP || state ==DOWN)
    {

    }
}
