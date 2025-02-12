# Automotive
## 1. GPIO
Sử dụng thư viên SPL chứa các hàm để cấu hình các ngoại vi và sử dụng. Các bước lần lượt khi sử dụng một ngoại vị: Cấp clock - Cấu hình - Sử dụng.  

Cấp clock cho bus chứa ngoại vi: 
- Sử dụng 3 bus: AHB, APB1, APB2
- Hàm "RCC_`<bus>`ClockCmd" giúp cung cấp clock cho ngoại vi nằm trên đường bus tương ứng trong tên hàm. Hàm này chứa 2 tham số:
    - Tham số dầu: Chỉ ngoại vị cần cấp clock trên đường bus tương ứng. Có cấu trúc chung là "RCC_`<bus>`Periph_<tên ngoại vi>" 
    - Tham số thứ hai: ENABLE hoăc DISABLE xác định có cấp clock cho ngoại vi không 
    VD: Cấp clock cho ngoại vị GPIOC
    `RCC_APB2ClockCmd(RCC_APB2Periph_GPIOC, ENABLE);`  

Cấu hình cho GPIO:  
- Khởi tạo các cấu hình cho GPIO: Sử dụng struct "GPIO_InitTypeDef" với các tham số:  
    - GPIO_Pin: Chân cần được cấu hình, nếu có nhiều chân cần cấu hình như nhau thì sử dụng toán tử | (OR). Các chân được khai báo trong thư viện dưới dạng sau:  
    ```
    #define GPIO_Pin_0                 ((uint16_t)0x0001)  /*!< Pin 0 selected */
    #define GPIO_Pin_1                 ((uint16_t)0x0002)  /*!< Pin 1 selected */
    #define GPIO_Pin_2                 ((uint16_t)0x0004)  /*!< Pin 2 selected */
    #define GPIO_Pin_3                 ((uint16_t)0x0008)  /*!< Pin 3 selected */
    #define GPIO_Pin_4                 ((uint16_t)0x0010)  /*!< Pin 4 selected */
    #define GPIO_Pin_5                 ((uint16_t)0x0020)  /*!< Pin 5 selected */
    #define GPIO_Pin_6                 ((uint16_t)0x0040)  /*!< Pin 6 selected */
    #define GPIO_Pin_7                 ((uint16_t)0x0080)  /*!< Pin 7 selected */
    #define GPIO_Pin_8                 ((uint16_t)0x0100)  /*!< Pin 8 selected */
    #define GPIO_Pin_9                 ((uint16_t)0x0200)  /*!< Pin 9 selected */
    #define GPIO_Pin_10                ((uint16_t)0x0400)  /*!< Pin 10 selected */
    #define GPIO_Pin_11                ((uint16_t)0x0800)  /*!< Pin 11 selected */
    #define GPIO_Pin_12                ((uint16_t)0x1000)  /*!< Pin 12 selected */
    #define GPIO_Pin_13                ((uint16_t)0x2000)  /*!< Pin 13 selected */
    #define GPIO_Pin_14                ((uint16_t)0x4000)  /*!< Pin 14 selected */
    #define GPIO_Pin_15                ((uint16_t)0x8000)  /*!< Pin 15 selected */
    #define GPIO_Pin_All               ((uint16_t)0xFFFF)  /*!< All pins selected */
    ```  
    - GPIO_Mode: chế độ muốn cấu hình bao gồm các giá trị sau:
    ```
    typedef enum {
        GPIO_Mode_AIN = 0x00,            //Analog Input
        GPIO_Mode_IN_FLOATING = 0x04,    //Input bình thường
        GPIO_Mode_IPD = 0x28,            //Input có điện trở kéo xuống    
        GPIO_Mode_IPU = 0x48,            //Input có điện trở kéo lên
        GPIO_Mode_Out_OD = 0x14,         //Output dạng open-drain
        GPIO_Mode_Out_PP = 0x10,         //Output dạng push-pull
        GPIO_Mode_AF_OD = 0x1C,          //Chế độ ngoại vi khác dạng open-drain
        GPIO_Mode_AF_PP = 0x18           //Chế độ ngoại vi khác dạng push-pull
    } GPIOMode_TypeDef;
    ```
    - GPIO_Speed: tốc độ đáp ứng của chân bao gồm 3 tốc độ 2MHz, 10MHz và 50MHz. Có định dạng: "GPIO_Speed_<tốc độ>".  
- Sau khi đã cấu hình cho GPIO sau thì bắt đầu khởi tạo GPIO và gắn cấu hình vào thanh ghi của GPIO muốn khởi tạo bằng hàm "GPIO_Init" với 2 tham số:  
    - Tham số đầu là tên ngoại vi muốn cấu hình  
    - Tham số thứ hai là con trỏ trỏ đến địa chỉ của struct "GPIO_InitTypeDef" cấu hình GPIO.
    VD: Khởi tạo và cấu hình chân PC0 và PC1 
    ```
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    ```  
Sử dụng GPIO: Các hàm trong thư viện dùng để điều khiển GPIO
```
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);            \\Đọc giá trị 1 bit trong cổng GPIO được cấu hình là INPUT (IDR), có thể đọc nhiều pin nhờ toán tử OR
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);                                 \\Đọc giá trị nguyên cổng GPIO được cấu hình là INPUT (IDR)
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);           \\Đọc giá trị 1 bit trong cổng GPIO được cấu hình là OUTPUT (ODR), có thể đọc nhiều pin nhờ toán tử OR
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);                                \\Đọc giá trị nguyên cổng GPIO được cấu hình là OUTPUT (ODR)
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);                        \\Cho giá trị 1 bit trong cổng GPIO = 1, có thể ghi nhiều pin nhờ toán tử OR
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);                      \\Cho giá trị 1 bit trong cổng GPIO = 0, có thể ghi nhiều pin nhờ toán tử OR
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);     \\Ghi giá trị "BitVal" vào 1 bit trong cổng GPIO, có thể ghi nhiều pin nhờ toán tử OR
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);                           \\Ghi giá trị "PortVal" vào nguyên cổng GPIO
```  
## 2. Ngắt & Timer
### NGẮT  
**Ngắt** là một sự kiện xảy ra trong hoặc ngoài MCU yêu cầu MCU phải dừng chương trình và thực thi chương trình ngắt (ISR).  
Mỗi loại ngắt trong MCU đều có một trình phục vụ riêng và mỗi một trình phục vụ ứng với một địa chỉ riêng trong hệ thống được gọi là vector ngắt.  
Cách ngắt thực hiện bên trong MCU:
- PC (Program counter): là con trỏ được chỉ tới địa chỉ lệnh đang được thực thi và luôn chỉ đến lệnh tiếp theo cũng là địa chỉ tiếp theo trong chương trình, khi xảy ra ngắt thì PC sẽ trỏ tới địa chỉ của hàm ngắt và hệ thống sẽ lưu địa chỉ trước đó vào MSP (main stack pointer) để khi thực hiện xong hàm ngắt thì sẽ lấy địa chỉ được lưu trong đó để quay lại thực thi tiếp chương trình đang dở
- MSP (main stack pointer): stack hoạt động dạng LIFO, trước khi nhảy tới địa chỉ hàm ngắt sẽ lưu lại địa chỉ trước đó vào MSC, cứ tiếp tục sau khi hoàn thành ngắt lấy phần tử đầu trong MSC để quay lại địa chỉ cũ để tiếp tục công việc
- Nếu đang thực thi trình phục vụ ngắt mà có 1 ngắt có độ ưu tiên cao hơn xảy ra thì hệ thống sẽ thực hiện lại bước lưu địa chỉ xong nhảy đến địa chỉ hàm ngắt có độ ưu tiên cao hơn  

Ba loại ngắt thường dung có thể lập trình được: ngắt ngoài, ngắt timer, ngắt truyền thông (UART, SPI, I2C)  

**Ngắt ngoài**  
Xảy ra khi có thay đổi điện áp trên các chân GPIO được cấu hình làm ngõ vào ngắt. Có 4 loại ngắt ngoài:  
- LOW: kích hoạt ngắt liên tục khi chân ở mức thấp
- HIGH: Kích hoạt liên tục khi chân ở mức cao
- RISING: Kích hoạt khi trạng thái trên chân chuyển từ thấp lên cao (cạnh lên)
- FALLING: Kích hoạt khi trạng thái trên chân chuyển từ cao xuống thấp (cạnh xuống)  

**Ngắt timer**  
Xảy ra khi giá trị trong thanh ghi đếm của timer bị tràn. Giá trị tràn được xác định bởi giá trị cụ thể trong thanh ghi đếm của timer. Đây là ngắt nội trong MCU, nên để có thể tạo được ngắt tiếp theo sau mỗi lần tràn thì phải reset lại giá trị thanh ghi  

**Ngắt truyền thông**  
Ngắt truyền thông xảy ra khi có sự kiện truyền/nhận dữ liệu giữa MCU với các thiết bị bên ngoài hay với MCU. Ngắt này sử dụng cho nhiều phương thức như SPI, I2C, UART,... nhằm đảm bảo việc truyền nhận chính xác.  
Ngắt truyền thông giúp MCU không cần kiểm tra liên tục trạng thái nhận dữ liệu mà ẫn có thể phản ứng ngay khi dữ liệu đến. Do MCU không thể xử lý đa luồng như CPU, nên ngắt giúp tối ưu hiệu suất và đảm bảo dữ liệu được xử lý đúng thời điểm.  
VD của ngắt truyền thông thường sử dụng:  
- UART với ngắt nhận dữ liệu (RX interrupt)
- SPI với ngắt truyền xong (TX complete interrupt)  
### TIMER  
**Timer** là mạch digital có nhiệm vụ đếm xung clock (đếm lên hoặc đếm xuống) để thực hiện các tác vụ liên quan đến thời gian như sau:
- Tạo độ trễ chính xác (delay)  
- Đo thời gian giữa các sự kiện  
- Tạo tín hiệu PWM  
- Điều khiển tốc độ động cơ , đèn LED  
- Tạo ngắt định kỳ  
Sử dụng struct "TIM_TimeBaseInitTypeDef" để cấu hình timer cơ bản sử dụng để đếm thời gian có các thành viên như sau:
```
typedef struct
{
  uint16_t TIM_Prescaler;    
  uint16_t TIM_CounterMode;     
  uint16_t TIM_Period;          
  uint16_t TIM_ClockDivision;     
  uint8_t TIM_RepetitionCounter;  
} TIM_TimeBaseInitTypeDef;
```  
- TIM_RepetitionCounter: sử dụng trong các timer 32bit (TIM1, TIM8)
- TIM_ClockDivision: Chia xung do MCU cung cấp cho bộ lọc đầu vào dùng để điều chỉnh tốc độ đếm với các tham số như giữ nguyên giá trị (chia cho 1), chia đôi và chia bốn:
    ```
    #define TIM_CKD_DIV1                       ((uint16_t)0x0000)
    #define TIM_CKD_DIV2                       ((uint16_t)0x0100)
    #define TIM_CKD_DIV4                       ((uint16_t)0x0200)
    #define IS_TIM_CKD_DIV(DIV) (((DIV) == TIM_CKD_DIV1) || \
                                ((DIV) == TIM_CKD_DIV2) || \
                                ((DIV) == TIM_CKD_DIV4))
    ```  
- TIM_Prescaler: Tham số dùng để xác định sau bao nhiêu chu kỳ xung clock thì thanh ghi CNT công lên 1
- TIM_Period: Đếm lên bao nhiêu lần sẽ tạo ra ngắt
- TIM_CounterMode: Kiểu đếm
    ```
    #define TIM_CounterMode_Up                 ((uint16_t)0x0000) \\ Đếm lên từ 0 -> period 
    #define TIM_CounterMode_Down               ((uint16_t)0x0010) \\ Đếm xuống từ prediod -> 0
    #define TIM_CounterMode_CenterAligned1     ((uint16_t)0x0020) \\ Ít sử dụng
    #define TIM_CounterMode_CenterAligned2     ((uint16_t)0x0040)
    #define TIM_CounterMode_CenterAligned3     ((uint16_t)0x0060)
    #define IS_TIM_COUNTER_MODE(MODE) (((MODE) == TIM_CounterMode_Up) || \
                                      ((MODE) ==  TIM_CounterMode_Down) || \
                                      ((MODE) == TIM_CounterMode_CenterAligned1) || \
                                      ((MODE) == TIM_CounterMode_CenterAligned2) || \
                                      ((MODE) == TIM_CounterMode_CenterAligned3))
    ```  
Sau khi cấu hình timer xong sử dụng hàm "TIM_TimeBaseInit" với 2 tham số:
- Tham số 1 là timer sử dụng vd: TIM2 
- Tham số 2 là con trỏ trỏ tới địa chỉ struct "TIM_TimeBaseInitTypeDef" vừa cấu hình  
Timer muốn chạy cần sự cho phép của MCU nên cần thêm hàm "TIM_Cmd" với hai tham số: timer sử dụng, ENABLE/DISABLE  
VD: Cấu hình và khởi tạo TIM2  
   ```
    TIM_TimeBaseInitTypeDef TIM_InitStruct;
	
    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1; //72MHz
	TIM_InitStruct.TIM_Prescaler = 7200 - 1; //0.1ms dem 1 lan
	TIM_InitStruct.TIM_Period = 0xFFFF; 
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
    TIM_Cmd(TIM2, ENABLE);
   ```   
Sử dụng timer: 
```
void TIM_SetCounter(TIM_TypeDef* TIMx, uint16_t Counter); //Đặt giá trị ban đầu cho timer
uint16_t TIM_GetCounter(TIM_TypeDef* TIMx); //Lấy giá trị đếm hiện tại của timer
```  
VD: Hàm delay  
```
void delay_ms(uint16_t time) {
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) < time*10); //time max = 6553
}
```  
## Community Protocol  
- Truyền nhận dữ lieu co bản là trao đổi tín hiệu điện áp giữa các chân với nhau -> dựa theo mức cao mức thấp -> Mức cao là bit 1, mức thấp bit 0  
- Khi chuyền chuỗi tách chuỗi thành các ký tự -> truyền 1 lần 1 ký tự
- Có 1 vấn đề khi truyền các từng bit là về 2 bit liền kề giống nhau thì bên nhận không phân biệt được bit mới hay cũ ->  cần các chuẩn giao tiếp giải quyết vấn đề bằng cách thống nhất thời gian truyền/nhận 1 bit dữ liệu
- Các chuẩn giao tiếp:
    - Nối tiếp: chuyển lần lượt các bit trên 1 đường dây
    - Song song: 1 bit chuyền trên 1 đường dây, chuyền 8 bit cần 8 đường dây -> tốn tài nguyên phần cứng nhưng tốc độ truyền nhanh hơn
- Các chế độ của một giao thức:
    - Đơn công: Mỗi thiết bị truyền hoặc nhận trong mọi thời điểm
    - Bán song công: Trong một thời điểm mỗi thiết bị chỉ được truyền hoặc nhận
    - Song công: Trong mọi thời điểm thiết bị đều có thể truyền và nhận dữ liệu cùng lúc  
### SPI  
**SPI (Serial Peripheral Interface)** là chuẩn giao tiếp nối tiếp đồng bộ. SPI hoạt động ở dạng song công và có thể cho phép 1 Master kết nối với nhiều Slave. Bao gồm 4 dây:
- SCK (Serial Clock): Tạo xung tín hiệu để đồng bộ truyền/nhận với các Slave, các Slave và Master sử dung chung 1 day SCK
- MISO (Master Input Slave output) : Master nhận dữ lieu từ Slave
- MOSI (Master Output Slave input) : Master truyền dữ lieu cho Slave
- SS (CS, NSS): Dây để xác định Slave cụ thể để giao tiếp -> để chọn Slave giao tiếp Master kéo xuống 0V, có bao nhiêu Slave thì có bây nhiêu day SS để nối với các Slave riêng biệt  
  
Nguyên lý hoạt động:
- Master kéo chân SS của Slave tương ứng xuống 0 để bắt đầu quá trình giao tiếp
- Master sẽ phát xung clock cứ mỗi chu kỳ clock sẽ là 1 bit Master truyền đi qua MOSI và nhận lại qua MISO
- Sau khi nhận xong 1 bit thanh ghi nhận của Slave và Master sẽ cập nhật giá trị nhận vào và dịch bit để sẵn sàng nhận bit tiếp theo 
- Lập lại quá trình đến khi truyền xong 8 bit trong thanh ghi  
SPI có 4 chế độ hoạt động phụ thuộc Clock Polarity – CPOL và Phase - CPHA:
- CPOL quyết định cực tính (hình dạng) của xung clock.
    - CPOL = 0: khi không truyền/nhận, SCK sẽ ở mức 0. Khi muốn truyền/nhận thì Master sẽ kéo chân SCK lên mức 1 theo chu kỳ
    - CPOL = 1: khi không truyền/nhận, SCK sẽ ở mức 1. Khi muốn truyền/nhận thì Master sẽ kéo chân SCK lên mức 0 theo chu kỳ (ngược lại với CPOL = 0)
- CPHA quyết định pha của xung clock, nơi mà bit sẽ được truyền đi trong một chu kỳ xung clock
    - CPHA = 0: bit được truyền/nhận ở cạnh đầu tiên trong chu kỳ xung clock, VD khi CPOL = 0 thì cạnh đầu tiên là cạnh lên, CPOL = 1 là cạnh xuống
    - CPHA = 1: bit được truyền/nhận ở cạnh thứ hai trong chu kỳ xung clock
### I2C  
**I2C (Inter-Intergrated Circuit)** là chuẩn giao tiếp nối tiếp, đồng bộ. I2C hoạt động ở dạng bán song công và có thể cho phép 1 Master kết nối với nhiều Slave. Bao gồm 2 dây:
- SPI (Serial Clock): Tạo xung tín hiệu để đồng bộ truyền/nhận dữ liệu với các Slave
- SDA (Serial Data): Chân chứa dữ liệu được truyền đi  

I2C hoạt động ở chế độ open-drain khi muốn điều khiển đường dây sẽ kéo xuống mức 0 đối với các trường hợp khác sẽ ở Floating (thả nổi) không có mức điện áp nào nên cần thiết kế 1 điện trở kéo lên để ở mức 1 nếu thiết bị không hỗ trợ  

Nguyên lý hoạt động: 
- Chuyền dữ liệu theo 1 khung (frame) cố định
- Trước khi truyền dữ liệu để thông báo cho Slave -> kéo SDA xuống 0 rồi kéo SCL xuống 0 để thông báo
- SCL = 1 bắt đầu truyền dữ liệu
- Tiếp theo, Master sẽ gửi 7 bit địa chỉ (dữ liệu 8 bit). Các bit địa chỉ được dùng để chọn Slave mà Master muốn giao tiếp. Với 7 bit địa chỉ thì trong I2C có thể chứa tối đa 127 Slave khác nhau (địa chỉ 0x00 là của Master).
- Sau khi gửi xong 7 bit địa chỉ thì Master tiếp tục gửi 1 bit R/W ngay sau bit địa chỉ cuối cùng. Bit này cho biết Master đang muốn gửi dữ liệu cho Slave (Write - 1) hay đọc dữ liệu từ Slave (Read - 0) có địa chỉ tương ứng.
- Theo sau mỗi một khung dữ liệu 8 bit sẽ là một bit xác nhận ACK/NACK. Bit này được bên nhận gửi đi cho bên gửi, cho biết rằng bên nhận đã nhận thành công (ACK - 0) hoặc không (NACK - 1).
- Khi Master gửi xong 8 bit trong khung dữ liệu, nó sẽ cho chạy timer 1 khoảng thời gian nhỏ, nếu không có Slave nào kéo chân SDA xuống mức 0 (chưa có ACK) khi timer chưa tràn thì Master sẽ hiểu là dữ liệu đã gửi không thành công (hoặc là không có Slave nào có địa chỉ tương ứng mà Master đã gửi).
- Sau khi Master phát hiện ACK từ Slave, nó sẽ tiến hành truyền/nhận khung dữ liệu 8 bit đầu tiên (MSB gửi trước, LSB gửi sau) tuỳ vào bit R/W ở khung dữ liệu đầu tiên. Sau đó, bên nhận sẽ gửi bit ACK.
- Sau khi đã phát hiện ACK thì sẽ tiếp tục truyền các khung dữ liệu cho tới khi hết dữ liệu.
- Khi muốn kết thúc quá trình truyền/nhận dữ liệu nào đó, Master sẽ tạo Stop Condition bằng cách thả đường SCL trước thả đường SDA lên mức 1.
### UART
**UART (Universal Asynchronous Receiver-Transmitter)** là một giao thức truyền thông phần cứng dùng giao tiếp nối tiếp không đồng bộ. UART hoạt động ở dạng song công và chỉ cho 1 máy gửi kết nối với 1 máy nhận. Bao gồm 2 dây:
- TX(Transmit): Chân truyền dữ liệu
- RX(Receive): Chân nhận dữ liệu  

Vì UART không có chân xung clock để đồng bộ dữ liệu giữa bên gửi và bên nhận, nên nó sẽ sử dụng timer để xác định khoảng thời gian giữa 2 bit được truyền/nhận. Bên gửi và bên nhận sẽ đồng nhất sau khoảng thời gian bao nhiêu thì bit tiếp theo sẽ được truyền/nhận. Khoảng thời gian này được tính sử dụng thông số là Baudrate.  

Baudrate là thông số cho biết bao nhiêu bit được gửi đi trong vòng 1s (đơn vị: bps - bits per second). Nhờ vào số bit được truyền trong 1s, ta tam suất để tính được sau bao nhiêu giây thì bit tiếp theo sẽ được gửi. Từ đó, bên gửi và bên nhận sẽ chỉnh sửa timer của mình sao cho khớp với khoảng thời gian này để đồng bộ được dữ liệu với nhau.
  
Nguyên lý hoạt động:
- Khi muốn truyền dữ liệu nào đó, bên gửi trước tiên sẽ tạo Start Condition bằng cách hạ đường TX từ mức 1 xuống mức 0 trong 1 khoảng thời gian đã đồng nhất ban đầu.
- Sau đó, bên gửi và bên nhận sẽ khởi động timer, bên gửi sau khi timer tràn sẽ gửi 1 bit, bên nhận sau khi timer tràn sẽ nhận 1 bit. Quá trình này diễn ra tới khi bit dữ liệu cuối cùng được gửi.
- Ngay sau khi các bit dữ liệu được gửi, có hoặc không 1 bit Parity được gửi. Bit Parity được dùng để kiểm tra lỗi trong các bit dữ liệu. Có 2 loại bit Parity là Parity chẵn (đảm bảo tổng số bit 1 trong các bit dữ liệu và bit Parity là số chẵn) và Parity lẻ (đảm bảo tổng số bit 1 trong các bit dữ liệu và bit Parity là số lẻ).
- Sau khi hoàn thành gửi Parity bit, bên gửi sẽ tạo Stop Condition bằng cách kéo đường TX từ 0 lên mức 1 trong khoảng thời gian từ 1 đến 2 lần khoảng thời gian đã đồng nhất ban đầu.