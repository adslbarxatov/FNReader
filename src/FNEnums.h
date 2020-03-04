// ����� ������������

// ��������� ���� ���������� ����������
enum FNDocumentTypes
	{
	// ����� � �����������
	Registration = 1,

	// ����� �� ��������� ���������� �����������
	RegistrationChange = 11,

	// �������� �����
	OpenSession = 2,

	// ����� � ������� ��������� ��������
	CurrentState = 21,

	// �������� ���
	Bill = 3,

	// ��� ���������
	CorrectionBill = 31,

	// ���
	Blank = 4,

	// ��� ���������
	CorrectionBlank = 41,

	// �������� �����
	CloseSession = 5,

	// �������� ����������� ������
	CloseFiscalStorage = 6,

	// ������������� ���������
	Confirmation = 7,

	// ����������� ��� ���������
	UnknownType = 255
	};

// ��������� ���� ����� ��
enum FNLifePhases									
	{
	// ��������������� �����
	FactoryMode = 0x00,

	// ���������� � ������������
	ReadyForLaunch = 0x01,

	// ���������� �����
	FiscalMode = 0x03,

	// ����� ������, �������� ����������
	AfterFiscalMode = 0x07,

	// ����� ������, ��������� ��������
	ArchiveClosed = 0x0F,

	// ����������� �����
	UnknownMode = 255
	};

// ��������� ���������� ������� � COM-������
enum SendAndReceiveResults							
	{
	// �������
	Ok = 0,

	// ������ ������
	WriteError = -11,

	// ������� ������
	ReadTimeout = -21,

	// ������ ������
	ReadError = -22,

	// ������������ ����� ��������� ���������
	AnswerLengthError = -23,

	// ������������ ����������� ����� ��������� ���������
	AnswerCRCError = -24,

	// ������� �����, ���������� ��������� �� ������
	AnswerLogicError = -31
	};

// �������������� TLV-����
enum TLVTags
	{
	// ���� ��� 1.0

	// ����� ����������� ��������� (1040)
	FiscalDocumentNumber = 0x0410,

	// ��� ������� (1021)
	CashierName = 0x03FD,

	// ��� ������������ (1048)
	UserName = 0x0418,

	// ����� �������� (1009)
	RegistrationAddress = 0x03F1,

	// ��� ��� (1017)
	OFD_INN = 0x03F9,

	// ��������� ����� ��� (1013)
	RegisterSerialNumber = 0x03F5,

	// ����� ��������� (1031)
	RealCashValue = 0x0407,

	// ����� ������������ (1081)
	ElectronicCashValue = 0x0439,

	// �������� �������� ������� (1059)
	PaymentObject = 0x0423,

	// ��������� ����� �� (1041)
	FNSerialNumber = 0x0411,

	// ��������������� ����� ��� (1037)
	RegistrationNumber = 0x040D,

	// ��� ������������ (1018)
	UserINN = 0x03FA,

	// ���� ���������� (1056)
	EncryptionFlag = 0x0420,

	// ���� ����������� ������ (1002)
	AutonomousFlag = 0x03EA,

	// ���� ������ � ������� ��������������� ���������� �������� (1001)
	AutomaticFlag = 0x03E9,

	// ���� ������ ����� (1109)
	ServiceFlag = 0x0455,

	// ���� ������������ ��� (1110)
	BlankFlag = 0x0456,

	// ���� ������ � ���� �������� (1108)
	InternetFlag = 0x0454,

	// ����� ������ ��������������� (1062)
	TaxFlags = 0x0426,

	// ����� �����, � ������� ������ �������� (1038)
	SessionNumber = 0x040E,

	// ������������ �������� ������� (1030)
	GoodName = 0x0406,

	// ��������� ������� ������ (1079)
	ItemCost = 0x0437,

	// ���������� ������ (1023)
	GoodsCount = 0x03FF,

	// ���� ��� 1.05

	// ��� ������� (1203)
	CashierINN = 0x04B3,

	// ������ ������� ���������� ���������� (1209)
	FFDVersion = 0x04B9,

	// E-mail/������� ���������� (1008)
	ClientAddress = 0x03F0,

	// ����� �������� (1187)
	RegistrationPlace = 0x04A3,

	// ���� ������ ������� ��

	// ���������� �������� (���������� �����, 65001)
	FNA_AutonomousArray = 0xFDE9,

	// ���������� �������� (����� �������� ������, 65002)
	FNA_OFDArray = 0xFDEA,

	// ���������� �������� (���������� �����, ��� 1.1, 65011)
	FNA_AutonomousArray_1_1 = 0xFDF3,

	// ���������� �������� (����� �������� ������, ��� 1.1, 65012)
	FNA_OFDArray_1_1 = 0xFDF4,

	// ����� � �����������
	FNA_Registration = Registration + 100,

	// ����� �� ��������� ���������� �����������
	FNA_RegistrationChange = RegistrationChange + 100,

	// �������� �����
	FNA_OpenSession = OpenSession + 100,

	// ����� � ������� ��������� ��������
	FNA_CurrentState = CurrentState + 100,

	// �������� ���
	FNA_Bill = Bill + 100,

	// ��� ���������
	FNA_CorrectionBill = CorrectionBill + 100,

	// ���
	FNA_Blank = Blank + 100,

	// ��� ���������
	FNA_CorrectionBlank = CorrectionBlank + 100,

	// �������� �����
	FNA_CloseSession = CloseSession + 100,

	// �������� ����������� ������
	FNA_CloseFiscalStorage = CloseFiscalStorage + 100,

	// ������������� ���������
	FNA_Confirmation = Confirmation + 100,

	// ���, ���������� � ���������� ������ �� ��� ����������
	ReadingFailure = 9999,

	// ����������� ���� 1.0 � 1.05

	// ����� �������� (���������) (1036)
	TerminalNumber = 0x040C,

	// ���� � ����� ��������� (1012)
	DocumentDateTime = 0x03F4,

	// ���������� ������� ��������� (1077)
	FiscalSignature = 0x0435,

	// ������� ��������������� (1101)
	RegistrationChangeCause = 0x044D,

	// ���������� ������� ��������� (���������, 1078)
	FiscalConfirmation = 0x0436,

	// ������� ������� (1054)
	OperationType = 0x041E,

	// ����� ������� (1020)
	OperationSumma = 0x03FC,

	// ���������� ��� (1055)
	AppliedTaxSystem = 0x041F,

	// ��� �������� ������� (��� ����������) (1162)
	GoodCode = 0x048A,

	// ���� �������� ��� (1193)
	GamesFlag = 0x04A9,

	// ���� ����������� ������� (1207)
	ExciseFlag = 0x04B7,

	// ������� �������� ������� (1212)
	ResultObject = 0x04BC,

	// ������� ������� ������� (1214)
	ResultMethod = 0x04BE,

	// ������ ��� (1199)
	NDS = 0x04AF,
	};
