# FNReader


Программа FNReader предназначена для чтения и обработки фискальных данных (ФД) из фискального накопителя (ФН).

***С версии 7.7 проект является частью приложения [TextToKKT](https://github.com/adslbarxatov/TextToKKT); данный репозиторий более не обновляется***


#

## Функциональность (на момент включения в состав TextToKKT)

1. Чтение данных из ФН с помощью физического или виртуального COM-порта и аппаратного адаптера.

2. Формирование выгрузки архива ФН в формате .fnc **в полном соответствии с пунктами 45 – 58 Приложения 2 к приказу ФНС России
«Форматы фискальных документов, обязательные к использованию» для версии ФФД 1.05**.

3. Обработка фискальных данных:
- Получение полного состояния и всех регистрационных данных ФН;
- Получение отдельного документа ФН;
- Получение полного отчёта по архиву ФН;
- Получение контрольной ленты за отдельную смену;
- Получение полного фискального (посменного) отчёта;
- Получение посменного отчёта по диапазону дат.

4. Сохранение архива ФН в файл в формате:
- Двоичных данных (доступен для последующего открытия и обработки в программе); **это – внутренний формат программы (.fsd),
  который не может быть использован при перерегистрации или снятии ККТ с учёта в личном кабинете ФНС**;
- Текстовой контрольной ленты;
- Табличных данных (доступен для обработки в Microsoft Office Excel).

5. ***Выполнение обмена с ОФД с параметрами подключения, получаемыми автоматически по данным последней регистрации / перерегистрации***.

6. Сброс МГМ (для технических целей).

7. Закрытие смены и архива ФН текущей датой или датой последнего документа ФН.

8. Чтение выгрузок архивов ФН в форматах .fnc и .fsd.

#

Чтение может быть выполнено тремя способами:

- *Полное чтение архива*. По его завершении становятся доступными все функции обработки данных.

- *Прямое чтение ФН*. В этом случае доступны функции чтения статуса ФН, отдельного документа и контрольной
ленты за смену (при условии, если функция поддерживается накопителем).

- *Формирование выгрузки с последующей её загрузкой*. Функция позволяет самостоятельно формировать файл
выгрузки ФН, который (аналогично созданным в других программах) может быть открыт в FNReader.

Независимо от варианта и настроек чтения состояние ФН запрашивается в максимально полном виде.

#

Детализация чтения также может быть:

- *Полной*. При этом из ФН считываются все основные текстовые (TLV) поля и квитанции подтверждения ОФД. Может занимать продолжительное
время. Наиболее старые версии ФН не поддерживают полную выгрузку.

- *Краткой*. При этом считываются только суммовые счётчики, временны́е метки, номера, фискальные признаки документов и состояния
отправки ОФД. Выполняется быстрее полного считывания (примерно в 2 раза).

&nbsp;



## Политика разработки и EULA

Данная [Политика (ADP)](https://vk.com/@rd_aaow_fdl-adp), её положения, заключение, EULA и способы применения
описывают общие правила, которым мы следуем во всех наших процессах разработки, вышедших в релиз приложениях
и реализованных идеях.
**Обязательна к ознакомлению всем участникам и пользователям перед использованием любого из продуктов лаборатории.
Загружая их, вы соглашаетесь с этой Политикой**
