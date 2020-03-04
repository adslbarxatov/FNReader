// Общие перечисления

// Возможные типы фискальных документов
enum FNDocumentTypes
	{
	// Отчёт о регистрации
	Registration = 1,

	// Отчёт об изменении реквизитов регистрации
	RegistrationChange = 11,

	// Открытие смены
	OpenSession = 2,

	// Отчёт о текущем состоянии расчётов
	CurrentState = 21,

	// Кассовый чек
	Bill = 3,

	// Чек коррекции
	CorrectionBill = 31,

	// БСО
	Blank = 4,

	// БСО коррекции
	CorrectionBlank = 41,

	// Закрытие смены
	CloseSession = 5,

	// Закрытие фискального режима
	CloseFiscalStorage = 6,

	// Подтверждение оператора
	Confirmation = 7,

	// Неизвестный тип документа
	UnknownType = 255
	};

// Возможные фазы жизни ФН
enum FNLifePhases									
	{
	// Технологический режим
	FactoryMode = 0x00,

	// Готовность к фискализации
	ReadyForLaunch = 0x01,

	// Фискальный режим
	FiscalMode = 0x03,

	// Архив закрыт, передача документов
	AfterFiscalMode = 0x07,

	// Архив закрыт, документы переданы
	ArchiveClosed = 0x0F,

	// Неизвестный режим
	UnknownMode = 255
	};

// Возможные результаты общения с COM-портом
enum SendAndReceiveResults							
	{
	// Успешно
	Ok = 0,

	// Ошибка записи
	WriteError = -11,

	// Таймаут чтения
	ReadTimeout = -21,

	// Ошибка чтения
	ReadError = -22,

	// Некорректная длина ответного сообщения
	AnswerLengthError = -23,

	// Некорректная контрольная сумма ответного сообщения
	AnswerCRCError = -24,

	// Получен ответ, содержащий сообщение об ошибке
	AnswerLogicError = -31
	};

// Обрабатываемые TLV-теги
enum TLVTags
	{
	// Теги ФФД 1.0

	// Номер фискального документа (1040)
	FiscalDocumentNumber = 0x0410,

	// Имя кассира (1021)
	CashierName = 0x03FD,

	// Имя пользователя (1048)
	UserName = 0x0418,

	// Адрес расчётов (1009)
	RegistrationAddress = 0x03F1,

	// ИНН ОФД (1017)
	OFD_INN = 0x03F9,

	// Заводской номер ККТ (1013)
	RegisterSerialNumber = 0x03F5,

	// Сумма наличными (1031)
	RealCashValue = 0x0407,

	// Сумма электронными (1081)
	ElectronicCashValue = 0x0439,

	// Название предмета расчёта (1059)
	PaymentObject = 0x0423,

	// Заводской номер ФН (1041)
	FNSerialNumber = 0x0411,

	// Регистрационный номер ККТ (1037)
	RegistrationNumber = 0x040D,

	// ИНН пользователя (1018)
	UserINN = 0x03FA,

	// Флаг шифрования (1056)
	EncryptionFlag = 0x0420,

	// Флаг автономного режима (1002)
	AutonomousFlag = 0x03EA,

	// Флаг работы в составе автоматического устройства расчётов (1001)
	AutomaticFlag = 0x03E9,

	// Флаг режима услуг (1109)
	ServiceFlag = 0x0455,

	// Флаг формирования БСО (1110)
	BlankFlag = 0x0456,

	// Флаг работы в сети Интернет (1108)
	InternetFlag = 0x0454,

	// Флаги систем налогообложения (1062)
	TaxFlags = 0x0426,

	// Номер смены, в которой создан документ (1038)
	SessionNumber = 0x040E,

	// Наименование предмета расчёта (1030)
	GoodName = 0x0406,

	// Стоимость единицы товара (1079)
	ItemCost = 0x0437,

	// Количество товара (1023)
	GoodsCount = 0x03FF,

	// Теги ФФД 1.05

	// ИНН кассира (1203)
	CashierINN = 0x04B3,

	// Версия формата фискальных документов (1209)
	FFDVersion = 0x04B9,

	// E-mail/телефон покупателя (1008)
	ClientAddress = 0x03F0,

	// Место расчётов (1187)
	RegistrationPlace = 0x04A3,

	// Теги файлов архивов ФН

	// Фискальный документ (автономный режим, 65001)
	FNA_AutonomousArray = 0xFDE9,

	// Фискальный документ (режим передачи данных, 65002)
	FNA_OFDArray = 0xFDEA,

	// Фискальный документ (автономный режим, ФФД 1.1, 65011)
	FNA_AutonomousArray_1_1 = 0xFDF3,

	// Фискальный документ (режим передачи данных, ФФД 1.1, 65012)
	FNA_OFDArray_1_1 = 0xFDF4,

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

	// Тег, сообщающий о внутренней ошибке ФН при считывании
	ReadingFailure = 9999,

	// Расширенные теги 1.0 и 1.05

	// Номер автомата (терминала) (1036)
	TerminalNumber = 0x040C,

	// Дата и время документа (1012)
	DocumentDateTime = 0x03F4,

	// Фискальный признак документа (1077)
	FiscalSignature = 0x0435,

	// Причина перерегистрации (1101)
	RegistrationChangeCause = 0x044D,

	// Фискальный признак сообщения (квитанция, 1078)
	FiscalConfirmation = 0x0436,

	// Признак расчёта (1054)
	OperationType = 0x041E,

	// Сумма расчёта (1020)
	OperationSumma = 0x03FC,

	// Применённая СНО (1055)
	AppliedTaxSystem = 0x041F,

	// Код товарной позиции (код маркировки) (1162)
	GoodCode = 0x048A,

	// Флаг азартных игр (1193)
	GamesFlag = 0x04A9,

	// Флаг подакцизных товаров (1207)
	ExciseFlag = 0x04B7,

	// Признак предмета расчёта (1212)
	ResultObject = 0x04BC,

	// Признак способа расчёта (1214)
	ResultMethod = 0x04BE,

	// Ставка НДС (1199)
	NDS = 0x04AF,
	};
