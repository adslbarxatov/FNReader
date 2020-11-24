# FNReader v 6.5


Программа FNReader предназначена для чтения и обработки фискальных данных (ФД) из фискального накопителя (ФН)


#

## Функциональность

На данный момент доступны следующие функции:

1. Чтение данных из ФН с помощью физического или виртуального COM-порта и аппаратного адаптера.

2. Обработка фискальных данных:
- Получение полного состояния и всех регистрационных данных ФН;
- Получение отдельного документа ФН;
- Получение полного архива ФН;
- Получение контрольной ленты за отдельную смену;
- Получение полного фискального (посменного) отчёта;
- Получение посменного отчёта по диапазону дат.

3. Сохранение архива ФН в файл в формате:
- Двоичных данных (доступен для последующего открытия и обработки в программе); имеет расширение .fsd;
**это – внутренний формат программы, который не может быть использован при перерегистрации или снятии ККТ с учёта в личном кабинете ФНС**.
- Текстовой контрольной ленты;
- Табличных данных (доступен для обработки в Microsoft Office Excel).

4. *Формирование выгрузки архива ФН в формате .fnc **в полном соответствии с пунктами 45 – 58 Приложения 2 к приказу ФНС России «Форматы фискальных документов, обязательные к использованию» (экспериментальная функция)***;

5. Сброс МГМ (для технических целей);

6. Закрытие смены и архива ФН текущей датой или датой последнего документа ФН;

7. Чтение выгрузок архивов ФН в формате .fnc.

#

Чтение может быть выполнено двумя способами:

- *Полное чтение архива*. По его завершении становятся доступны все функции обработки данных.

- *Прямое чтение ФН*. В этом случае доступны функции чтения статуса ФН, отдельного документа и контрольной ленты за смену (при условии,
если функция поддерживается накопителем).

- *Формирование выгрузки с последующей её загрузкой*. Это экспериментальная функция, позволяющая самостоятельно формировать файл архива ФН,
который аналогично созданным в других программах может быть открыт в FNReader.

Независимо от варианта и настроек чтения состояние ФН запрашивается в максимально полном виде.

#

Детализация чтения также может быть:

- *Полной*. При этом из ФН считываются все основные текстовые (TLV) поля и квитанции подтверждения ОФД. Может занимать продолжительное
время. Наиболее старые версии ФН не поддерживают полную выгрузку.

- *Неполной*. Пользователь может выбрать вариант без считывания текстовых полей или без считывания квитанций ОФД, что может сильно
ускорить процесс чтения.

- *Краткой*. При этом считываются только суммовые счётчики, временны́е метки, номера, фискальные признаки документов и состояния
отправки ОФД. Выполняется быстрее полного считывания (примерно в 2 раза).

&nbsp;



## Требования к оборудованию

Работа программы протестирована на всех моделях ФН из реестра. При соблюдении производителями существующего аппаратного протокола
чтения данных работа с имеющимися и будущими моделями ФН также возможна.

Работа программы возможна при наличии следующих аппаратных адаптеров:

- COM-UART переходник для подключения ФН к разъёму COM (RS-232) компьютера;

- USB-VCOM-UART переходник для подключения к разъёму USB компьютера. В этом случае потребуется установить драйвера устройства;
инструкции и ссылки для их установки предоставляются производителями таких переходников.

Для работы программы рекомендуется использовать операционную систему Windows 7 или более новую.
На ПК должна быть установлена среда .NET Framework 4.0 или новее (обязательно при работе на Windows XP).

&nbsp;



## Прочее

- Язык интерфейса и руководства пользователя: ru_ru.
- Список поддерживаемых TLV-тегов можно посмотреть [здесь](https://github.com/adslbarxatov/FNReader/blob/master/src/FNEnums.h).
- По вопросам предоставления доступа к приложению просим обращаться на [электронную почту](mailto://adslbarxatov@mail.ru).
- Срок действия каждого релиза программы ограничен с целью устранения устаревающих версий программы и обеспечения
постоянного соответствия программы актуальным изменениям в ФФД. Срок указан в заголовке окна приложения. По его истечении
необходимо получить новый экземпляр приложения для продолжения работы

&nbsp;



## Политика разработки и EULA

Данная [Политика (ADP)](https://vk.com/@rdaaow_fupl-adp), её положения, заключение, EULA и способы применения
описывают общие правила, которым мы следуем во всех наших процессах разработки, вышедших в релиз приложениях
и реализованных идеях.
**Обязательна к ознакомлению всем участникам и пользователям перед использованием любого из продуктов лаборатории.
Загружая их, вы соглашаетесь с этой Политикой**
