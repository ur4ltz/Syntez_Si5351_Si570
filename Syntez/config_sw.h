//  Конфиг для простого трансивер с одной ПЧ 11.059MHz
//  Полоса пропускания фильтра по уровню 3дб 11056950..11060000Гц
//  Первый гетеродин всегда выше частоты приема

#ifndef CONFIG_SW_H
#define CONFIG_SW_H

// необходимо раскоментировать требуемую моду (только одну!)

// режим прямого преобразования. частота формируется на 1ом выводе. установить
// CLK0_MULT в значение 1/2/4 в зависимости от коэффициента деления частоты гетеродина
// второй и третий гетеродины отключены
//#define MODE_DC

// режим прямого преобразования с формированием квадратурн
// частота формируется на выводах CLK0,CLK1 со сдвигом фаз 90град
// CLK2 отключен. Минимальная частота настройки 2MHz (по даташиту 4MHz) может зависеть от экземпляра Si5351
// режим работает только при использовании Si5351
//#define MODE_DC_QUADRATURE

// одна промежуточная частота. требуемая боковая формируется на счет переключения
// первого гетеродина с инверсией боковой либо без инверсии. второй гетеродин формируется на выходе CLK1
// тип КФ зависит от параметров SSBDetectorFreq_LSB/USB. если фильтр симметричный (определены две частоты SSBDetectorFreq_*)
// то частота первого гетеродина всегда сверху (меньше пораженок) а боковая выбирается изменением частоты второго гетеродина
#define MODE_SINGLE_IF

// аналогично MODE_SINGLE_IF но второй гетеродин генерируется на CLK1 при RX и
// на CLK2 в режиме TX
//#define MODE_SINGLE_IF_RXTX

// аналогично MODE_SINGLE_IF но в режиме передачи гетеродины комутируются,
// тоесть первый формируется на CLK1, а второй - на CLK0
// для трактов где необходимо переключение гетеродинов при смене RX/TX (например Аматор)
//#define MODE_SINGLE_IF_SWAP

// две промежуточные частоты. гетеродины формируются 1й - CLK0, 2й - CLK1, 3й - CLK2
// первый гетеродин всегда "сверху". выбор боковой полосы производится сменой частоты
// второго гетеродина (если заданы USB/LSB) или сменой частоты третьего гетеродина MODE_DOUBLE_IF
//#define MODE_DOUBLE_IF

// режим аналогичен MODE_DOUBLE_IF но в режиме передачи 2й и 3й гетеродины комутируются,
// тоесть второй формируется на CLK2, а третий - на CLK1
// для трактов с двумя промежуточными частотами где необходимо переключение
// гетеродинов при смене RX/TX
//#define MODE_DOUBLE_IF_SWAP23

// множители частоты на выходах. в случае необходимости получения на выводе 2/4 кратной
// частоты установить в соответствующее значение
const long CLK0_MULT = 1;
const long CLK1_MULT = 1;
const long CLK2_MULT = 1;

// Частота 2го (3го для двойного преобразования) гетеродина в детекторе SSB.
// Может быть определена как одна константа для верхнего/нижнего ската
// так и обе. При определении только одной константы изменение боковой полосы производится
// 1ым (2ым для двойного преобразования) гетеродином
// При определении обеих констант они должны находится на соответствующих скатах фидьтра.
// Если используется два раздельных фильтра для выделения LSB/USB то выбор их осуществляется
// с помощью сигнала BCPN_SB на порту дешифратора диапазонов

// значения для sb_mode - выбор боковой полосы. SBM_DSB в случае приема AM/FM - частота
// по центру фильтра и второй гетеродин выключен
#define SBM_LSB  0
#define SBM_USB  1
#define SBM_DSB  2

extern const struct _Modes {
  char* name;
  char cat_name;      // '0'-No mode, '1'-LSB, '2'-USB, '3'-CW, '4'-FM, '5'-AM
  uint8_t sb_mode;    // задает боковую
  uint8_t tx_enable;  // разрешена передача в этой моде
  int16_t   rx_shift; // сдвиг при приеме. нужно для CW
  uint32_t  freq[2];  // частота гетеродинов для SBM_LSB/SBM_USB или центр ПЧ для SBM_DSB
} Modes[];

// Для каждой моды приема необходимо определить как будет использоваться фильтр
// Если мода SBM_DSB то freq[0] задает центральную частоту в полосе пропускания фильтра. freq[1] при этом должна быть равна 0
// Если мода SBM_LSB/USB частоты freq задают соответственно частоты среза фильтра по уровню 3дб сверху/снизу
// Если необходимо запретить использование фильтра на каком-то из склонов (обычно из-за его пологости) то прописывается соответствующая частота равная 0
// Например: фильтр можно использовать только как LSB. При этом в зависимости от требуемой принимаемой боковой будет выбираться автоматически инверсии полосы
//   SBM_LSB,  true,    0, {11060000L+300, 0}}, \
//   SBM_USB,  true,    0, {11060000L+300, 0}}, \
// Например: в тракте установлено два ЭМФ - верхний и нижной которые переключаются при смене боковой
//   SBM_LSB,  true,    0, {0, 500000}}, \
//   SBM_USB,  true,    0, {500000, 0}}, \
// 
// В меню синтезатора есть возможно менять в небольших пределах для каждой моды частоты среза фильтра и сдвиг при приеме

#define   DEFINED_MODES \
  {"LSB", '1', SBM_LSB,  true,    0, {11060000L+300, 11056950L-300}}, \
  {"USB", '2', SBM_USB,  true,    0, {11060000L+300, 11056950L-300}}, \
  {"CW",  '3', SBM_USB,  false, 700, {11060000L-700, 11056950L+700}}, \
  {"AM",  '5', SBM_DSB,  false,   0, {11070000L, 0}}

// Первая промежуточная частота для трактов с двойным преобразованием частоты
// Используется только если выбрана одна из мод MODE_DOUBLE_*
// Если полоса шире чем принимаемый сигнал то надо определить IFreqEx
// Если полоса узкая и сигнал надо выровнять по одному из скатов то определяем IFreqEx_USB/LSB
//#define IFreqEx 45000000
//#define IFreqEx_LSB 45000000
//#define IFreqEx_USB 45000000

#endif
