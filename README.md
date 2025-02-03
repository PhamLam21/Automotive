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
- PC (Program counter): là con trỏ được chỉ tới địa chỉ lệnh đang được thực thi và luôn chỉ đến lệnh tiếp theo cũng là địa chỉ tiếp theo trong chương trình, khi xảy ra ngắt thì PC sẽ trỏ tới địa chỉ của hàm ngắt và hệ thống sẽ lưu địa chỉ trước đó vào MSP (main stack pointer) để khi thực hiện xong hàm ngắt thì sẽ lấy địa chỉ được lưu trong đó để quay lại thực thi tiếp chương trình đang dở.
- MSP (main stack pointer): stack hoạt động dạng LIFO, trước khi nhảy tới địa chỉ hàm ngắt sẽ lưu lại địa chỉ trước đó vào MSC, cứ tiếp tục sau khi hoàn thành ngắt lấy phần tử đầu trong MSC để quay lại địa chỉ cũ để tiếp tục công việc.
- Nếu đang thực thi trình phục vụ ngắt mà có 1 ngắt có độ ưu tiên cao hơn xảy ra thì hệ thống sẽ thực hiện lại bước lưu địa chỉ xong nhảy đến địa chỉ hàm ngắt có độ ưu tiên cao hơn.  
Ba loại ngắt thường dung có thể lập trình được: ngắt ngoài, ngắt timer, ngắt truyền thông (UART, SPI, I2C).  
**Ngắt ngoài**  
Xảy ra khi có thay đổi điện áp trên các chân GPIO được cấu hình làm ngõ vào ngắt. Có 4 loại ngắt ngoài:  
- LOW: kích hoạt ngắt liên tục khi chân ở mức thấp.
- HIGH: Kích hoạt liên tục khi chân ở mức cao.
- RISING: Kích hoạt khi trạng thái trên chân chuyển từ thấp lên cao (cạnh lên).
- FALLING: Kích hoạt khi trạng thái trên chân chuyển từ cao xuống thấp (cạnh xuống).  
**Ngắt timer**  
Xảy ra khi giá trị trong thanh ghi đếm của timer bị tràn. Giá trị tràn được xác định bởi giá trị cụ thể trong thanh ghi đếm của timer. Đây là ngắt nội trong MCU, nên để có thể tạo được ngắt tiếp theo sau mỗi lần tràn thì phải reset lại giá trị thanh ghi.  
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