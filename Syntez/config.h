#ifndef CONFIG_H
#define CONFIG_H

#define CAT_ENABLE
#define COM_BAUND_RATE  9600      // скорость обмена COM-порта

#define RIT_MAX_VALUE   1200      // максимальная расстройка

// раскоментировать установленные чипы
//#define VFO_SI5351
#define VFO_SI570

#define SI5351_XTAL_FREQ         250000000  // 0.1Hz resolution (10x mutiplier)
#define SI5351_CALIBRATION_FREQ  30000000   // частота на которой проводится калибровка Si5351

#define LSB 0
#define USB 1

// число диапазонов
#define BAND_COUNT 9

extern const struct _Bands {
  uint8_t   mc;
  long  start, startSSB, end;
  uint8_t sideband;
} Bands[];

#define DEFINED_BANDS \
  {160,  1810000L,  1840000L,  2000000L, LSB}, \
  {80,   3500000L,  3600000L,  3800000L, LSB}, \
  {40,   7000000L,  7045000L,  7200000L, LSB}, \
  {30,  10100000L,        0,  10150000L, USB}, \
  {20,  14000000L, 14100000L, 14350000L, USB}, \
  {17,  18068000L, 18110000L, 18168000L, USB}, \
  {15,  21000000L, 21150000L, 21450000L, USB}, \
  {12,  24890000L, 24930000L, 25140000L, USB}, \
  {10,  28000000L, 28200000L, 29700000L, USB}

// для режима general coverage
#define FREQ_MIN  1000000L
#define FREQ_MAX 30000000L

// список комманд трансивера. порядок важен!!!
enum {
  cmdNone = 0,
  cmdBandUp,   // переключение диапазонов или частоты
  cmdBandDown,
  cmdLock,     // Lock freq
  cmdVFOSel,   // VFO A/B
  cmdAttPre,   // переключает по кругу аттенюатор/увч
  cmdRIT,      // RIT
  cmdQRP,     // режим уменьшенной выходной мощности
  cmdHam,      // режим Ham band/General coverage. в режиме Ham кнопки cmdBandUp/Down переключают диапазоны
               // в режиме General coverage - изменяют частоту на +/-1MHz
  cmdZero,     // устанавливает частоту точно по еденицам кГц. 3623145->3623000
  cmdVFOEQ,    // VFO A=B
  cmdUSBLSB,   // выбор боковой USB/LSB
  cmdSplit,    // Split on/off
  cmdMenu
};

// необходимо раскоментировать требуемую моду (только одну!)

//*****************************************************************************************//

// режим прямого преобразования. частота формируется на 1ом выводе. установить
// CLK0_MULT в значение 1/2/4 в зависимости от коэффициента деления частоты гетеродина
// второй и третий гетеродины отключены
//#define MODE_DC

// режим прямого преобразования с формированием квадратурн
// частота формируется на выводах CLK0,CLK1 со сдвигом фаз 90град
// CLK2 отключен. Минимальная частота настройки 2MHz (по даташиту 4MHz) может зависеть от экземпляра Si5351
//#define MODE_DC_QUADRATURE

//*****************************************************************************************//

// одна промежуточная частота. требуемая боковая формируется на счет переключения
// первого гетеродина с инверсией боковой либо без инверсии. второй гетеродин формируется на выходе CLK1
// тип КФ зависит от параметров IFreq_LSB/IFreq_USB. если фильтр симметричный (определены две частоты IFreq)
// то частота первого гетеродина всегда сверху (меньше пораженок) а боковая выбирается изменением частоты второго гетеродина
//#define MODE_SINGLE_IF

// аналогично MODE_SINGLE_IF но второй гетеродин генерируется на CLK1 при RX и
// на CLK2 в режиме TX
//#define MODE_SINGLE_IF_RXTX

// аналогично MODE_SINGLE_IF_VFOSB но в режиме передачи гетеродины комутируются,
// тоесть первый формируется на CLK1, а второй - на CLK0
// для трактов где необходимо переключение гетеродинов при смене RX/TX
//#define MODE_SINGLE_IF_SWAP

//*****************************************************************************************//

// две промежуточные частоты. гетеродины формируются 1й - CLK0, 2й - CLK1, 3й - CLK2
// первый гетеродин всегда "сверху". выбор боковой полосы производится сменой частоты
// второго гетеродина для режимов MODE_DOUBLE_IF_USB/LSB, или сменой частоты третьего гетеродина MODE_DOUBLE_IF
// в режиме MODE_DOUBLE_IF второй гетеродин выше первой ПЧ
//#define MODE_DOUBLE_IF
//#define MODE_DOUBLE_IF_USB
#define MODE_DOUBLE_IF_LSB

// режим аналогичен MODE_DOUBLE_IF но в режиме передачи 2й и 3й гетеродины комутируются,
// тоесть второй формируется на CLK2, а третий - на CLK1
// для трактов с двумя промежуточными частотами где необходимо переключение
// гетеродинов при смене RX/TX
//#define MODE_DOUBLE_IF_SWAP23
//#define MODE_DOUBLE_IF_USB_SWAP23
//#define MODE_DOUBLE_IF_LSB_SWAP23

// множители частоты на выходах. в случае необходимости получения на выводе 2/4 кратной
// частоты установить в соответствующее значение
const long CLK0_MULT = 1;
const long CLK1_MULT = 1;
const long CLK2_MULT = 1;

// следующие дефайны определяют какой у нас фильтр - нижняя либо верхняя боковая
// они задают частоту второго (или третьего) гетеродина
// если фильтр имеет симметричные скаты (например мостовой) либо высокое подавление 
// по обеим скатам то раскоментарить и определить оба дефайна
#define IFreq_LSB   500000L

// первая промежуточная частота для трактов с двойным преобразованием частоты
const long IFreqEx = 45000000;

#endif
