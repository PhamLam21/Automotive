# Automotive

## 1. GPIO
Sử dụng thư viên SPL chứa các hàm để cấu hình các ngoại vi và sử dụng. Các bước lần lượt khi sử dụng một ngoại vị: Cấp clock - Cấu hình - Sử dụng.  

Cấp clock cho bus chứa ngoại vi: 
- Sử dụng 3 bus: AHB, APB1, APB2
- Hàm "RCC_<bus>ClockCmd" giúp cung cấp clock cho ngoại vi nằm trên đường bus tương ứng trong tên hàm. Hàm này chứa 2 tham số:
    - Tham số dầu: Chỉ ngoại vị cần cấp clock trên đường bus tương ứng. Có cấu trúc chung là "RCC_<bus>Periph_<tên ngoại vi>" 
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