# FNReader 5.2

Программа FNReader предназначена для чтения и обработки фискальных данных (ФД) из фискального накопителя (ФН).

#

На данный момент доступны следующие функции:

1. Чтение данных из ФН с помощью физического или виртуального COM-порта и аппаратного адаптера (см. далее).

2. Обработка фискальных данных:

    * Получение полного состояния и регистрационных данных ФН; автоопределение модели ФН и оператора фискальных данных;

    * Получение отдельного документа ФН;

    * Получение полного архива ФН;

    * Получение контрольной ленты за отдельную смену;

    * Получение полного фискального (посменного) отчёта;

    * Получение посменного отчёта по диапазону дат.

3. Сохранение архива ФН в файл в формате:

    * Двоичных данных (доступен для последующего открытия и обработки в программе); имеет расширение .fsd; **это – внутренний формат 
программы, который не может быть использован при перерегистрации или снятии ККТ с учёта в личном кабинете ФНС**.

    * Текстовой контрольной ленты;

    * Табличных данных (доступен для обработки в Microsoft Office Excel).

4. Сброс МГМ (для технических целей);

5. Чтение выгрузок архивов ФН в формате .fnc, создаваемом программами FNArc и FNTransfer **в полном соответствии с пунктами 45 – 48
Приложения 2 к приказу ФНС России «Форматы фискальных документов, обязательные к использованию»**.

#

Чтение может быть выполнено двумя способами:

- *Полное чтение архива*. По его завершении становятся доступны все функции обработки данных.

- *Прямое чтение ФН*. В этом случае доступны функции чтения статуса ФН, отдельного документа и контрольной ленты за смену (при условии,
если функция поддерживается накопителем).

Независимо от варианта и настроек чтения состояние ФН запрашивается в максимально полном виде.

#

Детализация чтения также может быть:

- *Полной*. При этом из ФН считываются все основные текстовые (TLV) поля и квитанции подтверждения ОФД. Может занимать продолжительное
время. Наиболее старые версии ФН не поддерживают полную выгрузку.

- *Неполной*. Пользователь может выбрать вариант без считывания текстовых полей или без считывания квитанций ОФД, что может сильно
ускорить процесс чтения.

- *Краткой*. При этом считываются только суммовые счётчики, временны́е метки, номера, фискальные признаки документов и состояния
отправки ОФД. Выполняется быстрее полного считывания (примерно в 2 раза).

#

Работа программы протестирована на всех моделях ФН из реестра. При соблюдении производителями существующего аппаратного протокола
чтения данных работа с имеющимися и будущими моделями ФН также возможна.

Работа программы возможна при наличии следующих аппаратных адаптеров:

- COM-UART переходник для подключения ФН к разъёму COM (RS-232) компьютера;

- USB-VCOM-UART переходник для подключения к разъёму USB компьютера. В этом случае потребуется установить драйвера устройства;
инструкции и ссылки для их установки предоставляются производителями таких переходников.

#

Для работы программы рекомендуется использовать операционную систему Windows 7 или более новую.

На ПК должна быть установлена среда .NET Framework 4.0 или новее (обязательно при работе на Windows XP).

Язык интерфейса и руководства пользователя: ru_ru

#

Список поддерживаемых TLV-тегов:

```
	// Стандартные теги

	// Флаг работы в составе автоматического устройства расчётов (1001)
	AutomaticFlag = 0x03E9,

	// Флаг автономного режима (1002)
	AutonomousFlag = 0x03EA,

	// E-mail/телефон покупателя (1008)
	ClientAddress = 0x03F0,

	// Адрес расчётов (1009)
	RegistrationAddress = 0x03F1,

	// Дата и время документа (1012)
	DocumentDateTime = 0x03F4,

	// Заводской номер ККТ (1013)
	RegisterSerialNumber = 0x03F5,

	// ИНН ОФД (1017)
	OFD_INN = 0x03F9,

	// ИНН пользователя (1018)
	UserINN = 0x03FA,

	// Сумма расчёта (1020)
	OperationSumma = 0x03FC,

	// Имя кассира (1021)
	CashierName = 0x03FD,

	// Количество товара (1023)
	GoodsCount = 0x03FF,

	// Наименование предмета расчёта (1030)
	GoodName = 0x0406,

	// Сумма наличными (1031)
	RealCashValue = 0x0407,

	// Номер автомата (терминала) (1036)
	TerminalNumber = 0x040C,

	// Регистрационный номер ККТ (1037)
	RegistrationNumber = 0x040D,

	// Номер смены, в которой создан документ (1038)
	SessionNumber = 0x040E,

	// Номер фискального документа (1040)
	FiscalDocumentNumber = 0x0410,

	// Заводской номер ФН (1041)
	FNSerialNumber = 0x0411,

	// Имя пользователя (1048)
	UserName = 0x0418,

	// Признак расчёта (1054)
	OperationType = 0x041E,

	// Применённая СНО (1055)
	AppliedTaxSystem = 0x041F,

	// Флаг шифрования (1056)
	EncryptionFlag = 0x0420,

	// Название предмета расчёта (1059)
	PaymentObject = 0x0423,

	// Флаги систем налогообложения (1062)
	TaxFlags = 0x0426,

	// Фискальный признак документа (1077)
	FiscalSignature = 0x0435,

	// Фискальный признак сообщения (квитанция, 1078)
	FiscalConfirmation = 0x0436,

	// Стоимость единицы товара (1079)
	ItemCost = 0x0437,

	// Сумма электронными (1081)
	ElectronicCashValue = 0x0439,

	// Причина перерегистрации (1101)
	RegistrationChangeCause = 0x044D,

	// Флаг работы в сети Интернет (1108)
	InternetFlag = 0x0454,

	// Флаг режима услуг (1109)
	ServiceFlag = 0x0455,

	// Флаг формирования БСО (1110)
	BlankFlag = 0x0456,

	// Код товарной позиции (код маркировки) (1162)
	GoodCode = 0x048A,

	// Место расчётов (1187)
	RegistrationPlace = 0x04A3,

	// Флаг азартных игр (1193)
	GamesFlag = 0x04A9,

	// Ставка НДС (1199)
	NDS = 0x04AF,

	// ИНН кассира (1203)
	CashierINN = 0x04B3,

	// Флаг подакцизных товаров (1207)
	ExciseFlag = 0x04B7,

	// Версия формата фискальных документов (1209)
	FFDVersion = 0x04B9,

	// Признак предмета расчёта (1212)
	ResultObject = 0x04BC,

	// Признак способа расчёта (1214)
	ResultMethod = 0x04BE,

	// Теги файлов архивов ФН

	// Отчёт о регистрации
	FNA_Registration = Registration + 100,

	// Отчёт об изменении реквизитов регистрации
	FNA_RegistrationChange = RegistrationChange + 100,

	// Открытие смены
	FNA_OpenSession = OpenSession + 100,

	// Отчёт о текущем состоянии расчётов
	FNA_CurrentState = CurrentState + 100,

	// Кассовый чек
	FNA_Bill = Bill + 100,

	// Чек коррекции
	FNA_CorrectionBill = CorrectionBill + 100,

	// БСО
	FNA_Blank = Blank + 100,

	// БСО коррекции
	FNA_CorrectionBlank = CorrectionBlank + 100,

	// Закрытие смены
	FNA_CloseSession = CloseSession + 100,

	// Закрытие фискального режима
	FNA_CloseFiscalStorage = CloseFiscalStorage + 100,

	// Подтверждение оператора
	FNA_Confirmation = Confirmation + 100,

	// Фискальный документ (автономный режим, 65001)
	FNA_AutonomousArray = 0xFDE9,

	// Фискальный документ (режим передачи данных, 65002)
	FNA_OFDArray = 0xFDEA,

	// Фискальный документ (автономный режим, ФФД 1.1, 65011)
	FNA_AutonomousArray_1_1 = 0xFDF3,

	// Фискальный документ (режим передачи данных, ФФД 1.1, 65012)
	FNA_OFDArray_1_1 = 0xFDF4,

	// Тег, сообщающий о внутренней ошибке ФН при считывании
	ReadingFailure = 9999,

```

#

По вопросам предоставления доступа к приложению просим обращаться на [электронную почту](mailto://adslbarxatov@scat-m.ru)

#

Мы формализовали нашу [Политику разработки приложений (ADP)](https://vk.com/@rdaaow_fupl-adp).
Настоятельно рекомендуем ознакомиться с ней перед использованием наших продуктов
