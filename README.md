# rts-project


모던 OpenGL RTS 프로젝트

사용 라이브러리 : assimp, glm, glfw, SDL, SOIL

#### 카메라 조절 방법
- 숫자패드4568
- 방향키
- 마우스 휠로 조정
#### 유닛 지정
- 드래그, 클릭으로 유닛 지정
- a키로 공격
- 우클릭으로 이동

다음 개발 : AI, ssao, bloom, 멀티쓰레드 랜더링

![rtsimage](https://github.com/rlatkddn212/rts-project/blob/master/RTS.png)

### 지금까지 구현한 주요 기술

1. height map, alpha map 사용하여 지형 생성
2. 스켈레톤 캐릭터 애니메이션
3. 픽킹, 드래그 (캐릭터 선택)
4. A* 길찾기
5. 전장의 안개
6. 미니맵
7. 디퍼드 랜더링
8. 퐁세이딩
9. 그림자 매핑



### 프로젝트 진행

| 아이템               | 진척도 | 우선 순위 |
| -------------------- | ------ | --------- |
| 맵 생성              | 100%   | 1         |
| 맵에 오브젝트 추가   | 100%   | 1         |
| 캐릭터 애니메이션    | 100%   | 1         |
| 이펙트               | 40%     | 2         |
| 전장의 안개          | 80%     | 2         |
| 카메라               | 50%    | 1         |
| 키보드 마우스 입출력 | 100%   | 1         |
| 미니맵               | 100%     | 2         |
| 경로 이동            | 100%     | 1         |
| 유닛 충돌 이동       | 70%     | 2         |
| ray(픽킹)            | 100%   | 1         |
| 맵 에디터            | 0%     | 5         |
| 건물 건설            | 50%     | 3         |
| 유닛 생산            | 0%     | 3         |
| 자원 추가            | 0%     | 3         |
| 아이템 추가          | 0%     | 4         |
| 업그레이드           | 0%     | 4         |
| 스킬                 | 0%     | 4         |
| 네트워크             | 0%     | 3         |
| text 랜더링          | 80%    | 1         |
| 그림자               | 0%     | 3         |
| 라이팅               | 80%     | 3         |
| 전투 시스템          | 40%     | 2         |
| xml 파서             | 0%     | 4         |
| 부대 지정            | 0%     | 5         |
| 논 타겟 스킬(충돌)   | 0%     | 4         |
| pve (AI)             | 0%     | 4         |
| UI                   | 0%     | 4         |
| 음악                 | 0%     | 5         |



### 의식의 흐름

1. 멀티쓰레드 구조

- 게임 객체들에 update 루프와 OpenGL Render 루프를 다른 쓰레드에서 동작하도록 수정
- Update루프가 완료되면 Rendering에 필요한 데이터를 Render쓰레드로 보낼 수 있도록 Message Queue 방식으로 전달
- 애니메이션 처리에 가장 많은 CPU 자원이 필요하므로 따로 쓰레드로 분할해서 처리하는 구조 변경 필요

- 현재 각 Object 클래스가 OpenGL 함수를 포함하고 있는 상태이므로 이를 분할된 구조로 변경 필요

2. ShadowMap 적용

- 현재 구조로 안타깝게도 Shadow Map 적용이 어려워 보인다. (완)

- 현재 각 Object가 Camera를 받아 랜더링 된다. 각 오브젝트에 대해 ShadowMap 생성 함수를 추가하거나, Object Rendering을 하기전 Shader를 설정 하는 방법이 있겠다.
- 현재 각 오브젝트마다 shader 설정, vao 설정, Texture 설정을 하기 때문에 OpenGL Call이 많다. Object Rendering을 하기전 Shader 설정하는 방법이 좋긴한데, 그렇게 일반화 시켜버리면 나중에 각 오브젝트마다 셰이더를 따로 설정하고 싶을 때 수정하기 힘들지 않을지
- shadow map 적용 완료했지만 fog of war가 model 단위에서 light를 처리하고 있기 때문에 매우 어색하게 보여지고 있다.
- fog of war를 defered shader에 포함시키거나 fog of war에 색상을 어둡게 처리해야 할 것 같다.

3. JSON, XML 적용

- 아마 json을 사용할 것 같다. (속도 측면에서 좋음)
- 게임에 여러 Resource 관리를 Json 형태로 저장시킨다.

4. 맵 에디터

- 스타크래프트 map 에디터 처럼 만들면 좋겠는데 map에 대한 정보는 json 파일로 나뉘어져 에 저장될 듯하다. 
- 지형 데이터와 여러 이벤트, 유닛, 스킬 많은 정보들을 json형태로 분할하여 맵 파일로 관리 되어야한다.

5. 모델, 애니메이션 데이터

- Assimp를 사용하여 모델 데이터를 읽고 있다.
- 모델 데이터에 새로운 포맷을 개발 할 경우 파서를 새로 개발해야한다. 모델을 읽은 후 성능에는 지정 없다. 새로운 포맷 개발은 load 속도 개선만 있을 뿐이다. 즉 필요 없을 수도 있다.
- 애니메이션 데이터를 사용할 때 느리다. Assimp로 애니메이션 데이터를 읽은 후 cache 하여 사용하도록한다.

6. 내비게이션 메시 적용

- 현재 Tile base로 A*  path finding을 하고 있다.
- 2가지 문제점이 있는데, 이동이 어색하고, 성능이 느리다.
- 내비게이션 메시를 구현하여 2가지 문제점을 해결 하도록한다.
- 구현 후에도 frame 드랍이 있을 경우 frame안에 탐색을 마칠 수 있도록 A*에 탐색 노드 수에 제한을 둬야할 것 같다. 

7. 네트워크

- c++ iocp나 epoll을 사용할 것 같다.
- p2p를 할지, 서버 클라를 할지 고민, p2p의 경우 udp로 연결 설정, nat 환경에 대한 대응이 필요하다. 서버 클라의 경우 서버 성능, 분산처리, 대역폭만 고민하면 될 듯
- 아니면 마인크래프트처럼 유저가 서버를 여는 방식으로 구현할 수 있다. 이러면 신경 쓸 일이 줄어든다.

8. 디자인 리소스

- 3d max, 포토샵을 배워야한다.
- 리소스를 따로 구매하여 사용한다해도 모델을 다듬어 사용 할 수 있어야한다.

9. 기획

- RTS기반이 완성되면 그 때 생각 해본다.
