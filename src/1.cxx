// Этот файл описывает функционал внешней (по отношению к интерфейсной части) дамповки ФН
// Является подключаемым (included)

// Команды прямого доступа к архиву
uchar x63Call[] = { 0x04, 0x05, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };	// Команда запроса страницы архива
#define x63Call_AL	0x0106

uchar x4901Call[] = { 0x04, 0x03, 0x00, 0x49, 0x01, 0x00, 0x00, 0x00 };				// Команда запроса TLV по номеру регистрации
uchar x4902Call[] = { 0x04, 0x06, 0x00, 0x49, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };	// Команда запроса TLV по номеру документа
#define x49Call_AL	0x08

uchar x4ACall[] = { 0x04, 0x05, 0x00, 0x4A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };	// Команда получения TLV
uchar x4BCall[] = { 0x04, 0x01, 0x00, 0x4B, 0x03, 0x02 };							// Команда завершения получения TLV
// Длина ответа определяется в коде

uchar x45Call[] = { 0x04, 0x05, 0x00, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };	// Команда обычного запроса TLV по номеру документа
#define x45Call_AL	0x0A



// Макросы
#define FIRST_PAGE		(fnType & 0x02) ? 1024 : 512	
// Отличие между ФН-15 и ФН-36

#define DOCUMENT_SENT	((currentHeaderTag == FNA_OFDArray) && (fnadd.Str.SendState > 0))

#define WRITE_TAG_HEADER(t,l)			fncth.Str.TagNumber = t;\
	fncth.Str.TagFieldSize = l;\
	fwrite (fncth.Ptr, 1, sizeof (fncth), FO);

#define WRITE_DOCUMENT_HEADER(size)		fncth.Str.TagNumber = currentHeaderTag;\
	fncth.Str.TagFieldSize = size;\
	if DOCUMENT_SENT\
		fncth.Str.TagFieldSize += 0x30;\
	fwrite (fncth.Ptr, 1, sizeof (fncth), FO);\
	WRITE_TAG_HEADER (fnadd.Str.DocumentType + 100, size - 4)

#define WRITE_DOCUMENT_DATE_TIME	WRITE_TAG_HEADER (DocumentDateTime, sizeof (dateTime))\
	dateTime = DateToUNIX (fnadd.Ptr + 10);\
	fwrite (&dateTime, 1, 4, FO);

#define WRITE_FISCAL_DOCUMENT_NUMBER		WRITE_TAG_HEADER (FiscalDocumentNumber, sizeof (fnadd.Str.DocumentNumber))\
	fwrite (&(fnadd.Str.DocumentNumber), 1, 4, FO);

#define WRITE_FISCAL_SIGNATURE	WRITE_TAG_HEADER (FiscalSignature, 6)\
	fputc (fnadd.Str.DocumentType, FO);\
	fputc (sizeof (fnadd.Str.DocumentFiscalSign), FO);\
	fwrite (&(fnadd.Str.DocumentFiscalSign), 1, 4, FO);

#define WRITE_SESSION_NUMBER	WRITE_TAG_HEADER (SessionNumber, 4)\
	fwrite (&(fnast.Str.SessionNumber), 1, 2, FO);\
	fputc (0x00, FO);\
	fputc (0x00, FO);

#define WRITE_CONFIRMATION	if DOCUMENT_SENT {\
	\
	WRITE_TAG_HEADER (FNA_Confirmation, 0x2C)\
	\
	WRITE_TAG_HEADER (OFD_INN, sizeof (fnart.Str.OFDINN))\
	fwrite (fnart.Str.OFDINN, 1, sizeof (fnart.Str.OFDINN), FO);\
	\
	WRITE_TAG_HEADER (DocumentDateTime, sizeof (fnadd.Str.SendUNIXDate))\
	fwrite (&(fnadd.Str.SendUNIXDate), 1, 4, FO);\
	\
	WRITE_TAG_HEADER (FiscalConfirmation, 2 * sizeof (fnadd.Str.SendFiscalSign))\
	fwrite (&(fnadd.Str.SendFiscalSign), 1, 8, FO);\
	fputc (0x00, FO); fputc (0x00, FO); fputc (0x00, FO); fputc (0x00, FO);\
	fputc (0x00, FO); fputc (0x00, FO); fputc (0x00, FO); fputc (0x00, FO);\
	}



// Цвета
#define SET_COLOR(c)			SetConsoleTextAttribute (cout, c);
#define SET_HEADER_COLOR		SET_COLOR (0x1B)	// Голубой
#define SET_MAIN_TEXT_COLOR		SET_COLOR (0x1F)	// Белый
#define SET_SECOND_TEXT_COLOR	SET_COLOR (0x17)	// Светло-серый
#define SET_WARNING_COLOR		SET_COLOR (0x1E)	// Жёлтый
#define SET_OK_COLOR			SET_COLOR (0x1A)	// Зелёный
#define SET_ERROR_COLOR			SET_COLOR (0x1C)	// Красный
#define SET_BACK_COLOR			SET_COLOR (0x11)	// Тёмно-синий

// Имя последнего файла выгрузки
static schar fncFilePath[SmallBufferSize];

// Метод формирует файл .fnc из ФН
FNR_API void CreateFNCEx (schar *COMPort, uchar MakeArchiveDumpOnly)
	{
	// Переменные
	ulong currentDocument = 0,
		dateTime = 0;
	uchar currentRegistration = 1,
		oldPosition, 
		currentPosition = 1;		// Сразу размер

	HANDLE cout;
	struct _CONSOLE_SCREEN_BUFFER_INFO cinCoords;

	schar msg[SmallBufferSize], arcFilePath[SmallBufferSize];
	uint msgOffset;
	FILE *FI = NULL, *FO = NULL;
	sint c;

	union FNC_Header fnch;
	union FNC_TagHeader fncth;
	ulong currentPage;
	uint currentHeaderTag;		// 65001 или 65002 для ФФД 1.05
	uchar fnType;

	union FNA_DocumentDefault fnadd;
	union FNA_RegistrationTail fnart;
	union FNA_RegistrationTail_1_1 fnart_1_1;
	union FNA_SessionTail fnast;
	union FNA_BillTail fnabt;
	union FNA_ClosingTail fnact;
	union FNA_StatusTail fnatt;

	// Запуск и настройка консольного окна
	if (!AllocConsole ())
		return;
	freopen ("CONOUT$", "w", stdout);
	freopen ("CONIN$", "r", stdin);

	SetConsoleTitleA (LIB_NAME);

	cout = GetStdHandle (STD_OUTPUT_HANDLE);
	SET_BACK_COLOR
	system ("cls");

	// Отображение заголовка и исходных данных
	SET_HEADER_COLOR
	printf (" \x11 %s \x10\n   %s\n\n", LIB_NAME, LIB_CN);

	SET_MAIN_TEXT_COLOR
	if (MakeArchiveDumpOnly)
		printf (" \x10 €­ЁжЁ «Ё§ жЁп Їа®жҐ¤гал д®а¬Ёа®ў ­Ёп д ©«  ARC\n");
	else
		printf (" \x10 €­ЁжЁ «Ё§ жЁп Їа®жҐ¤гал д®а¬Ёа®ў ­Ёп д ©«  FNC\n");

	SET_SECOND_TEXT_COLOR
	printf ("   Џ®ав: %s\n\n", COMPort);

	// Запуск процедуры
	if (FNInterfaceEx ((uchar *)COMPort) != 0)
		{
		sprintf (msg, " \x13 ЌҐ г¤ «®бм ®вЄалвм Ї®ав %s\n\n", COMPort);
		goto err;
		}

	// Первичное заполнение
	memset (fnch.Str.FileName, ' ', sizeof (fnch.Str.FileName));
	memset (fnch.Str.ApplicationName, ' ', sizeof (fnch.Str.ApplicationName));
	fnch.Str.CRC32 = -1;

	msg[0] = '\0';
	sprintf (msg, "%s", LIB_NAME);
	memcpy (fnch.Str.ApplicationName, msg, strlen (msg));

	// Запрос состояния
	if (SendAndReceiveMessage (x30Call, x30Call_AL) != Ok)
		{
		sprintf (msg, " \x13 ЌҐ г¤ «®бм Їа®зЁв вм бв вгб ”Ќ\n\n");
		goto snrerr;
		}

	// Формирование состояния
	fnch.Str.DocumentsCount = ((ulong)buffer[33] << 24) | ((ulong)buffer[32] << 16) | ((ulong)buffer[31] << 8) | (ulong)buffer[30];
	fnch.Str.FFDVersion = 0;	// Определяется и пишется позднее
	memcpy (fnch.Str.FNSerialNumber, buffer + 14, sizeof (fnch.Str.FNSerialNumber));
	
	// Получение версии и размера ФН
	
	// Можно решить эту задачу, используя команды 0x6C и 0x71, основываясь на следующих примерных отличиях в ПО ФН:

	// Тип		Дамп команды 0x6C															Память (0x71)	Результат (0x6C)
	// MGM		04 13 00 00 66 6E 20 64 65 62 75 67 20 76 20 31 2E 33 37 00 00 73 DE A4		256/256 Mb		fn debug v 1.37
	// 1.0		04 13 00 80 66 6E 5F 76 5F 31 5F 30 20 20 20 20 20 20 20 00 01 76 0F B1		512/256 Mb		fn_v_1_0
	// 1.1		04 13 00 00 66 6E 5F 76 5F 31 5F 31 20 20 20 20 20 20 20 00 01 D5 06 7E		256/256 Mb???	fn_v_1_1
	// 1.1/36	04 13 00 00 66 6E 5F 76 5F 31 5F 31 5F 32 20 20 20 20 20 00 01 D7 ED 42		512/512 Mb		fn_v_1_1_2

	// А можно сделать надёжный и быстрый костыль
	fnType = GetFNVersionAndSize (fnch.Str.FNSerialNumber);
	currentPage = FIRST_PAGE;

	// Обработка условий
	SET_MAIN_TEXT_COLOR
	printf (" \x10 ”Ќ ь%s (%s, %s) Ї®¤Є«озс­\n", BuildString (14, 16), (fnType & 0x01) ? "1.1" : "1.0",
		(fnType & 0x02) ? "36" : "13/15");
	if ((buffer[4] == FiscalMode) || (buffer[4] == AfterFiscalMode))
		{
		SET_WARNING_COLOR
		printf (" ! ” §  ¦Ё§­Ё: 0x%02X ( аеЁў ®вЄалв Ё«Ё Ґбвм ­ҐЇҐаҐ¤ ­­лҐ ”„)\n", buffer[4]);
		}

	else if (buffer[4] <= ReadyForLaunch)
		{
		SET_WARNING_COLOR
		printf (" ! ” §  ¦Ё§­Ё: 0x%02X (”Ќ ­Ґ дЁбЄ «Ё§Ёа®ў ­)\n", buffer[4]);
		
		sprintf (msg, " ! ЂаеЁў ”Ќ Їгбв\n\n");
		goto err;
		}
	
	else if (buffer[4] == ArchiveClosed)
		{
		SET_OK_COLOR
		printf ("   ” §  ¦Ё§­Ё: 0x%02X ( аеЁў § Єалв)\n", buffer[4]);
		}
	
	else
		{
		SET_ERROR_COLOR
		printf (" \x13 ” §  ¦Ё§­Ё: 0x%02X\n", buffer[4]);

		sprintf (msg, " \x13 ЌҐЁ§ўҐбв­ п д §  ¦Ё§­Ё ”Ќ\n\n");
		goto err;
		}

	if (buffer[7])
		{
		SET_WARNING_COLOR
		printf (" ! ‘¬Ґ­  ®вЄалв \n\n");
		}
	else
		{
		SET_OK_COLOR
		printf ("   ‘¬Ґ­  § Єалв \n\n");
		}

	// Запрос номера смены
	if (SendAndReceiveMessage (x10Call, x10Call_AL) != Ok)
		{
		fnch.Str.SessionNumber = 0;
		}
	else
		{
		fnch.Str.SessionNumber = (uint)buffer[5] | ((uint)buffer[6] << 8);
		}

	// Запрос регистрационного номера
	if (SendAndReceiveMessage (x40Call, x40Call_AL) != Ok)
		{
		sprintf (msg, " \x13 ЌҐ г¤ «®бм Їа®зЁв вм бв вгб ”Ќ\n\n");
		goto snrerr;
		}

	memcpy (fnch.Str.RegistrationNumber, buffer + 31, sizeof (fnch.Str.RegistrationNumber));

	// Сборка имён файлов
	msg[0] = '\0';
	SHGetSpecialFolderPathA (NULL, msg, CSIDL_DESKTOP, FALSE);
	msgOffset = strlen (msg);
	msg[msgOffset++] = '\\';

	memcpy (msg + msgOffset, fnch.Str.FNSerialNumber, sizeof (fnch.Str.FNSerialNumber));
	msg[msgOffset + sizeof (fnch.Str.FNSerialNumber)] = '\0';
	sprintf (fncFilePath, "%s.fnc", msg);
	sprintf (arcFilePath, "%s.arc", msg);

	// Инициализация записи в файлы
	if ((FI = fopen (arcFilePath, "wb")) == NULL)
		{
		sprintf (msg, " \x13 ЌҐ г¤ «®бм б®§¤ вм ўбЇ®¬®Ј вҐ«м­л© д ©«\n\n");
		goto err;
		}
	if ((FO = fopen (fncFilePath, "wb")) == NULL)
		{
		sprintf (msg, " \x13 ЌҐ г¤ «®бм б®§¤ вм д ©« %s\n\n", fncFilePath + msgOffset);
		goto err;
		}

	SET_MAIN_TEXT_COLOR
	printf (" \x10 ”®а¬ЁагҐвбп ўбЇ®¬®Ј вҐ«м­л© д ©«\n");

	// Сброс всех побочных состояний
	SendAndReceiveMessage (x06Call, x06Call_AL);	

	// Фискация курсора на свободной строке
	GetConsoleScreenBufferInfo (cout, &cinCoords);	
	cinCoords.dwCursorPosition.X = 0;
	SET_SECOND_TEXT_COLOR

	// Выгрузка архива во вспомогательный файл
	while (currentPage < fnch.Str.DocumentsCount / 2 + (FIRST_PAGE))
	// Явно завышенное ограничение – отсечка на случай проблем
		{
		// Запрос страницы
		x63Call[4] = currentPage & 0xFF;
		x63Call[5] = (currentPage >> 8) & 0xFF;
		x63Call[6] = (currentPage >> 16) & 0xFF;
		x63Call[7] = (currentPage >> 24) & 0xFF;
		SetCRC16 (x63Call);
		if (SendAndReceiveMessage (x63Call, x63Call_AL) != Ok)
			{
			sprintf (msg, "\n \x13 ЌҐ г¤ «®бм Їа®зЁв вм бва ­Ёжг  аеЁў  ”Ќ ь%u\n\n", currentPage);
			goto snrerr;
			}

		// Прогон по количеству документов на странице
		oldPosition = 0;
		while (oldPosition <= currentPosition)	// Манипуляция основана на переполнении
			{
			oldPosition = currentPosition;
			currentPosition += buffer[4 + currentPosition];

			currentDocument++;
			}

		// Защита
		if (currentDocument > fnch.Str.DocumentsCount)
			currentDocument = fnch.Str.DocumentsCount;

		// Сброс в файл
		fwrite (buffer + 4, 1, 256, FI);	// Длина страницы на команде 0x63 – 256 байт

		// Следующая ситуация возможна только тогда, когда последним элементом страницы оказался
		// не заполнитель (FF), а последний документ архива; при этом его конец оказался
		// на следующей странице, которую теперь и нужно дочитать
		if ((currentDocument == fnch.Str.DocumentsCount) && (oldPosition != (currentPosition + 1) % 0x100))
			{
			currentPage++;
			continue;
			}

		// Отображение результата с пропусками
		// (иначе имеет место торможение за счёт низкой скорости обновления консоли)
		if ((currentPage % 0x20 == 0) || (currentDocument == fnch.Str.DocumentsCount))
			{
			SetConsoleCursorPosition (cout, cinCoords.dwCursorPosition);
			printf ("   ‘зЁв ­® ¤®Єг¬Ґ­в®ў: %u Ё§ %u (бва ­Ёж  0x%04X)", currentDocument, fnch.Str.DocumentsCount, currentPage);
			}
		currentPage++;

		// Контроль
		if (currentDocument >= fnch.Str.DocumentsCount)
			break;
		}

	// Первичный сброс завершён. Запускаем обработку
	if (MakeArchiveDumpOnly)
		{
		printf ("\n\n");
		sprintf (msg, " ! ‘®§¤ ­  в®«мЄ® ­Ґ®Ўа Ў®в ­­ п ўлЈаг§Є   аеЁў  ”Ќ\n\n");
		goto err;
		}

	fclose (FI);
	FI = fopen (arcFilePath, "rb");

	SET_MAIN_TEXT_COLOR
	printf ("\n\n \x10 ”®а¬ЁагҐвбп д ©« %s\n", fncFilePath + msgOffset);

	GetConsoleScreenBufferInfo (cout, &cinCoords);	// Фискация курсора на свободной строке
	cinCoords.dwCursorPosition.X = 0;
	SET_SECOND_TEXT_COLOR

	// Запись имени файла в заголовок
	memcpy (fnch.Str.FileName, fncFilePath + msgOffset, strlen (fncFilePath) - msgOffset);

	// Запись документов
	for (currentDocument = 1; currentDocument <= fnch.Str.DocumentsCount; currentDocument++)
		{
		// Получение документа
		fread (fnadd.Ptr, 1, sizeof (fnadd), FI);
		if (((fnadd.Ptr[11] | (fnadd.Ptr[10] << 4)) & 0xFF) > LDD1)
			fnadd.Ptr[0] = 0xFF;

		switch (fnadd.Str.DocumentType)
			{
			// Отчёт о регистрации / перерегистрации
			case Registration:
			case RegistrationChange:
				fread (fnart.Ptr, 1, sizeof (fnart), FI);
				if (fnType & 0x1)
					fread (fnart_1_1.Ptr, 1, sizeof (fnart_1_1), FI);

				currentHeaderTag = ((fnart.Str.Mode & 0x02) ? FNA_AutonomousArray : FNA_OFDArray);
				AlignOFDINN (&fnadd, &fnart);	// Исправление на настоящий ИНН ОФД для документов подтверждения

				// Вместо стандартной записи запрашивается дамп TLV
				x4901Call[5] = currentRegistration;
				SetCRC16 (x4901Call);

				if (SendAndReceiveMessage (x4901Call, x49Call_AL) != Ok)
					{
					sprintf (msg, "\n \x13 ЌҐ г¤ «®бм Їа®зЁв вм ¤ ­­лҐ аҐЈЁбва жЁЁ ь%u\n\n", currentRegistration);
					goto snrerr;
					}

				x4ACall[6] = buffer[4];
				x4ACall[7] = buffer[5];
				SetCRC16 (x4ACall);

				if (SendAndReceiveMessage (x4ACall, buffer[4] | (buffer[5] << 8)) != Ok)
					{
					sprintf (msg, "\n \x13 ЌҐ г¤ «®бм Їа®зЁв вм ¤ ­­лҐ аҐЈЁбва жЁЁ ь%u\n\n", currentRegistration);
					goto snrerr;
					}
				currentRegistration++;
				//SendAndReceiveMessage (x4BCall, 6);

				// Получение последней использованной версии ФФД
				fnch.Str.FFDVersion = FindFFDVersion (buffer + 4 + sizeof (union FNA_TLVHeader), 
					bufferLength - sizeof (union FNA_TLVHeader) - 16);

				// Контроль версии ФФД
				if (fnch.Str.FFDVersion > FFDV)
					{
					sprintf (msg, "\n \x13 ‚ҐабЁЁ ””„ ­®ўҐҐ, зҐ¬ 1.05, ­  ¤ ­­л© ¬®¬Ґ­в ­Ґ Ї®¤¤Ґа¦Ёў овбп\n\n");
					goto err;
					}

				// Запись в файл
				fncth.Str.TagNumber = currentHeaderTag;
				fncth.Str.TagFieldSize = bufferLength - sizeof (union FNA_TLVHeader) - 6 + 44;	// Длина остальных реквизитов регистрации
				if DOCUMENT_SENT
					fncth.Str.TagFieldSize += 0x30;
				fwrite (fncth.Ptr, 1, sizeof (fncth), FO);
				
				WRITE_TAG_HEADER (fnadd.Str.DocumentType + 100, bufferLength - sizeof (union FNA_TLVHeader) - 16)
				fwrite (buffer + 4 + sizeof (union FNA_TLVHeader), 1, fncth.Str.TagFieldSize, FO);

				// Далее
				WRITE_CONFIRMATION

				WRITE_TAG_HEADER (FNASignature, 0x0A)
				fwrite (buffer + bufferLength - 0x0A - 2, 1, 0x0A, FO);

				WRITE_TAG_HEADER (FNARegistrationProtocol, sizeof (union FNA_TLVHeader))
				fwrite (buffer + 4, 1, sizeof (union FNA_TLVHeader), FO);

				break;

			// Открытие смены
			case OpenSession:
			case CloseSession:
				fread (fnast.Ptr, 1, sizeof (fnast), FI);

				// Запись в файл
				if (fnadd.Str.DocumentType == CloseSession)
					{ 
					WRITE_DOCUMENT_HEADER (0x26)
					}
				else
					{ 
					WRITE_DOCUMENT_HEADER (0x2B)

					WRITE_TAG_HEADER (AutonomousFlag, 1)
					fputc ((fnart.Str.Mode & 0x2) >> 1, FO);
					}

				WRITE_DOCUMENT_DATE_TIME

				WRITE_TAG_HEADER (SessionNumber, 4)
				fwrite (&(fnast.Str.SessionNumber), 1, 2, FO);
				fputc (0x00, FO);
				fputc (0x00, FO);

				WRITE_FISCAL_DOCUMENT_NUMBER

				WRITE_FISCAL_SIGNATURE

				WRITE_CONFIRMATION
				break;

			// Кассовый чек
			case Bill:
			case Blank:
			case CorrectionBill:
			case CorrectionBlank:
				fread (fnabt.Ptr, 1, sizeof (fnabt), FI);
				for (currentPosition = 5; currentPosition > 1; currentPosition--)	// Запрещает поле нулевой длины
					if (fnabt.Ptr[currentPosition] != 0)
						break;

				// Запись в файл
				if ((fnadd.Str.DocumentType == Bill) || (fnadd.Str.DocumentType == Blank))
					{ WRITE_DOCUMENT_HEADER (0x3B + currentPosition) }
				else
					{ WRITE_DOCUMENT_HEADER (0x27 + currentPosition) }

				WRITE_DOCUMENT_DATE_TIME

				WRITE_TAG_HEADER (OperationType, 1)
				fputc (fnabt.Str.OperationType, FO);

				WRITE_TAG_HEADER (OperationSumma, currentPosition)
				fwrite (fnabt.Ptr + 1, 1, currentPosition, FO);

				WRITE_FISCAL_DOCUMENT_NUMBER

				if ((fnadd.Str.DocumentType == Bill) || (fnadd.Str.DocumentType == Blank))
					{
					WRITE_TAG_HEADER (FNSerialNumber, sizeof (fnch.Str.FNSerialNumber))
					fwrite (fnch.Str.FNSerialNumber, 1, sizeof (fnch.Str.FNSerialNumber), FO);
					}

				WRITE_FISCAL_SIGNATURE

				WRITE_CONFIRMATION
				break;

			// Отчёт о текущем состоянии расчётов
			case CurrentState:
				fread (fnatt.Ptr, 1, sizeof (fnatt), FI);

				// Запись в файл
				WRITE_DOCUMENT_HEADER (0x2E)

				WRITE_DOCUMENT_DATE_TIME
				
				WRITE_TAG_HEADER (UnsentDocumentsCount, 4)
				fwrite (&(fnatt.Str.UnsentDocumentsCount), 1, 4, FO);

				WRITE_TAG_HEADER (FirstUnsentDocumentDate, 4)
				fnatt.Str.FirstUnsentDocumentDateHour = fnatt.Str.FirstUnsentDocumentDateMinute = 0;	// Согласно ФФД
				dateTime = DateToUNIX (fnatt.Ptr + 4);
				fwrite (&dateTime, 1, 4, FO);

				WRITE_FISCAL_DOCUMENT_NUMBER

				WRITE_FISCAL_SIGNATURE

				WRITE_CONFIRMATION
				break;

			// Закрытие фискального режима
			case CloseFiscalStorage:
				fread (fnact.Ptr, 1, sizeof (fnact), FI);

				// Вместо стандартной записи запрашивается дамп TLV
				x4902Call[5] = currentDocument & 0xFF;
				x4902Call[6] = (currentDocument >> 8) & 0xFF;
				x4902Call[7] = (currentDocument >> 16) & 0xFF;
				x4902Call[8] = (currentDocument >> 24) & 0xFF;
				SetCRC16 (x4902Call);

				if (SendAndReceiveMessage (x4902Call, x49Call_AL) != Ok)
					{
					sprintf (msg, "\n \x13 ЌҐ г¤ «®бм Їа®зЁв вм ¤ ­­лҐ ¤®Єг¬Ґ­в  ь%u\n\n", currentDocument);
					goto snrerr;
					}

				x4ACall[6] = buffer[4];
				x4ACall[7] = buffer[5];
				SetCRC16 (x4ACall);

				if (SendAndReceiveMessage (x4ACall, buffer[4] | (buffer[5] << 8)) != Ok)
					{
					sprintf (msg, "\n \x13 ЌҐ г¤ «®бм Їа®зЁв вм ¤ ­­лҐ ¤®Єг¬Ґ­в  ь%u\n\n", currentDocument);
					goto snrerr;
					}
				//SendAndReceiveMessage (x4BCall, 6);

				// Запись общего заголовочного тега
				fncth.Str.TagNumber = currentHeaderTag;
				fncth.Str.TagFieldSize = 2 * (bufferLength - sizeof (union FNA_TLVHeader) + 6) + 6;
				if DOCUMENT_SENT
					fncth.Str.TagFieldSize += 0x30;
				if (fnart.Str.Mode & 0x1)
					fncth.Str.TagFieldSize -= 6;
				fwrite (fncth.Ptr, 1, sizeof (fncth), FO);
				
				// Начало внутреннего тега
				WRITE_TAG_HEADER (fnadd.Str.DocumentType + 100, bufferLength - sizeof (union FNA_TLVHeader) - 10 - 6 -
					(fnart.Str.Mode & 0x1) * 6)

				// Репликация буфера
				memcpy (buffer2, buffer, BufferSize);
				buffer2Length = bufferLength;

				// При активном шифровании пробуем получить данные стандартным путём
				if (fnart.Str.Mode & 0x1)
					{
					x45Call[4] = currentDocument & 0xFF;
					x45Call[5] = (currentDocument >> 8) & 0xFF;
					x45Call[6] = (currentDocument >> 16) & 0xFF;
					x45Call[7] = (currentDocument >> 24) & 0xFF;
					SetCRC16 (x45Call);

					if (SendAndReceiveMessage (x45Call, x45Call_AL) != Ok)
						{
						sprintf (msg, "\n \x13 ЌҐ г¤ «®бм Їа®зЁв вм ¤ ­­лҐ ¤®Єг¬Ґ­в  ь%u\n\n", currentDocument);
						goto snrerr;
						}

					WRITE_TAG_HEADER (fnadd.Str.DocumentType, buffer2Length - sizeof (union FNA_TLVHeader) - 10 - 16)

					while (SendAndReceiveMessage (x46Call, x4647Call_AL) == Ok)
						fwrite (buffer + 4, 1, (buffer[1] | (buffer[2] << 8)) - 1, FO);
					}

				// иначе можно использовать ускоренный вариант
				else
					{
					fwrite (buffer2 + 4 + sizeof (union FNA_TLVHeader), 1, fncth.Str.TagFieldSize, FO);
					}

				// Завершение
				WRITE_CONFIRMATION

				WRITE_TAG_HEADER (FNAClosingProtocol, buffer2Length - 6)
				fwrite (buffer2 + 4, 1, fncth.Str.TagFieldSize, FO);
				break;

			default:
				sprintf (msg, "\n \x13 „®Єг¬Ґ­в ь%u Ё¬ҐҐв ­ҐЁ§ўҐбв­л© Ё«Ё ­ҐЇ®¤¤Ґа¦Ёў Ґ¬л© вЁЇ\n\n", currentDocument);
				goto err;
			}

		// Отображение с пропусками
		if ((currentDocument % 100 == 0) || (currentDocument == fnch.Str.DocumentsCount))
			{
			SetConsoleCursorPosition (cout, cinCoords.dwCursorPosition);
			printf ("   ‡ ЇЁб ­® ¤®Єг¬Ґ­в®ў: %u Ё§ %u", currentDocument, fnch.Str.DocumentsCount);
			}
		}

	// Формирование завершено
	fclose (FI);
	fclose (FO);

	// Замена промежуточного файла
	if (remove (arcFilePath) || rename (fncFilePath, arcFilePath))
		{
		sprintf (msg, " \x13 ЌҐ г¤ «®бм § ўҐаиЁвм ЇаҐ®Ўа §®ў ­ЁҐ ўбЇ®¬®Ј вҐ«м­®Ј® д ©« \n\n");
		goto err;
		}

	SET_MAIN_TEXT_COLOR
	printf ("\n\n \x10 Џа®ўҐаЄ  д ©« , § ўҐаиҐ­ЁҐ Їа®жҐ¤гал...\n\n");

	// Сборка окончательного файла
	FI = fopen (arcFilePath, "rb");
	if ((FO = fopen (fncFilePath, "wb")) == NULL)
		{
		sprintf (msg, " \x13 ЌҐ г¤ «®бм б®§¤ вм ўбЇ®¬®Ј вҐ«м­л© д ©«\n\n");
		goto err;
		}
	
	fwrite (fnch.Ptr, 1, sizeof (fnch.Ptr), FO);
	while ((c = fgetc (FI)) >= 0)
		fputc (c, FO);

	fclose (FI);
	fclose (FO);

	// Вычисление CRC32
	fnch.Str.CRC32 = GetCRC32ForFNC (fncFilePath);

	// Запись CRC32
	if (remove (fncFilePath))
		{
		sprintf (msg, " \x13 ЌҐ г¤ «®бм § ўҐаиЁвм ЇаҐ®Ўа §®ў ­ЁҐ ўбЇ®¬®Ј вҐ«м­®Ј® д ©« \n\n");
		goto err;
		}

	FI = fopen (arcFilePath, "rb");
	if ((FO = fopen (fncFilePath, "wb")) == NULL)
		{
		sprintf (msg, " \x13 ЌҐ г¤ «®бм б®§¤ вм ўбЇ®¬®Ј вҐ«м­л© д ©«\n\n");
		goto err;
		}
	
	fwrite (fnch.Ptr, 1, sizeof (fnch.Ptr), FO);
	while ((c = fgetc (FI)) >= 0)
		fputc (c, FO);

	fclose (FI);
	fclose (FO);

	if (remove (arcFilePath))
		{
		sprintf (msg, " \x13 ЌҐ г¤ «®бм § ўҐаиЁвм ЇаҐ®Ўа §®ў ­ЁҐ ўбЇ®¬®Ј вҐ«м­®Ј® д ©« \n\n");
		goto err;
		}

	// Завершено
	SET_OK_COLOR
	printf (" \x10 ” ©« FNC гбЇҐи­® бд®а¬Ёа®ў ­\n\n");

	goto normal;

	// Обработка ошибок
snrerr:
	// Сообщение об ошибке в окне
	ShowSendAndReceiveErrorMessage (SSAREM (10));

err:
	// Сообщение об ошибке в консоли
	if ((msg[1] == '\x13') || (msg[2] == '\x13'))
		SET_ERROR_COLOR
	else
		SET_WARNING_COLOR
	printf (msg);

normal:
	// Закрытие всех дескрипторов
	CloseEx ();
	if (FI)	fclose (FI);
	if (FO)	fclose (FO);

	// Завершение работы
	SET_SECOND_TEXT_COLOR
	printf ("   ");
	system ("Pause");
	
	FreeConsole ();
	}

// Преобразует время архивного документа в UNIX
ulong DateToUNIX (uchar *ArchiveDateTime)
	{
	struct tm timestamp;

	if ((ArchiveDateTime[0] == 0) || (ArchiveDateTime[1] == 0) || (ArchiveDateTime[2] == 0))
		return 0;

	timestamp.tm_year = ArchiveDateTime[0] + 100;
	timestamp.tm_mon = ArchiveDateTime[1] - 1;
	timestamp.tm_mday = ArchiveDateTime[2];
	timestamp.tm_hour = ArchiveDateTime[3];
	timestamp.tm_min = ArchiveDateTime[4];
	timestamp.tm_sec = 0;
	timestamp.tm_isdst = timestamp.tm_wday = timestamp.tm_yday = 0;

	return _mkgmtime (&timestamp);
	}

// Метод ищет тег 1209 в массиве и определяет версию ФФД
uchar FindFFDVersion (uchar *RegistrationTLV, uint TLVSize)
	{
	uint i = 4;

	while (i < TLVSize - 4)
		{
		if ((RegistrationTLV[i] == (FFDVersion & 0xFF)) &&
			(RegistrationTLV[i + 1] == ((FFDVersion >> 8) & 0xFF)) &&
			(RegistrationTLV[i + 2] == 1) && (RegistrationTLV[i + 3] == 0))
			return RegistrationTLV[i + 4];

		i += ((uint)RegistrationTLV[i + 2] | ((uint)RegistrationTLV[i + 3] << 8)) + 4;
		}

	return 1;	// ФФД 1.0 beta
	}

// Метод выравнивает ИНН ОФД по левому краю, если это необходимо
void AlignOFDINN (union FNA_DocumentDefault *FNADD, union FNA_RegistrationTail *FNART)
	{
	uchar i;

	for (i = 0; i < 5; i++)
		{
		FNART->Str.OFDINN[2 * i] = (FNADD->Str.OFD_INN_BCD[i] >> 4) + 0x30;
		FNART->Str.OFDINN[2 * i + 1] = (FNADD->Str.OFD_INN_BCD[i] & 0xF) + 0x30;
		}
	FNART->Str.OFDINN[10] = FNART->Str.OFDINN[11] = ' ';
	}

// Метод получает версию и размер ФН: b0 = 1/1.1, b1 = 15/36
uchar GetFNVersionAndSize (uchar *FNSerial)
	{
	uchar result = 0;
	/*schar serial[17];

	memcpy (serial, FNSerial, 16);
	serial[16] = '\0';

	if (strstr (GetFNName (serial), "1.1"))
		result |= 0x01;
	if (strstr (GetFNName (serial), "(36)"))
		result |= 0x02;*/

	if ((memcmp (FNSerial, "87100001", 8) != 0) &&
		(memcmp (FNSerial, "87110001", 8) != 0) &&
		(memcmp (FNSerial, "8712000100", 10) != 0) &&
		(memcmp (FNSerial, "8712000101", 10) != 0) &&
		(memcmp (FNSerial, "87150001", 8) != 0) &&
		(memcmp (FNSerial, "87160001", 8) != 0) &&
		(memcmp (FNSerial, "9999", 4) != 0))
		result |= 0x01;
	if ((memcmp (FNSerial, "87110001", 8) == 0) ||
		(memcmp (FNSerial, "92810001", 8) == 0) ||
		(memcmp (FNSerial, "92834403", 8) == 0) ||
		(memcmp (FNSerial, "92874403", 8) == 0) ||
		(memcmp (FNSerial, "92524403", 8) == 0))
		result |= 0x02;

	return result;
	}

// Метод возвращает имя последнего сформированного файла .fnc
FNR_API schar *GetLastCreatedFNCNameEx (void)
	{
	return fncFilePath;
	}
