// Общий заголовок
#include "FNReaderLib.h"
//#define EvaluateTable

// Этот файл описывает метод расчёта CRC16 CCITT с полиномом 0x1021

// Константы
#define TableLength			0x100		// Длина таблицы
#define Polynomial			0x1021		// Значение полинома

// Переменные
uint crc, crc_i, crc_e;

#ifdef EvaluateTable
	uint table[TableLength];			// Таблица
#else
	uint table[] = {
		0, 4129, 8258, 12387, 16516, 20645, 24774, 28903, 33032, 37161, 41290, 45419, 49548, 53677, 57806, 61935, 
		4657, 528, 12915, 8786, 21173, 17044, 29431, 25302, 37689, 33560, 45947, 41818, 54205, 50076, 62463, 58334, 
		9314, 13379, 1056, 5121, 25830, 29895, 17572, 21637, 42346, 46411, 34088, 38153, 58862, 62927, 50604, 54669, 
		13907, 9842, 5649, 1584, 30423, 26358, 22165, 18100, 46939, 42874, 38681, 34616, 63455, 59390, 55197, 51132, 
		18628, 22757, 26758, 30887, 2112, 6241, 10242, 14371, 51660, 55789, 59790, 63919, 35144, 39273, 43274, 47403, 
		23285, 19156, 31415, 27286, 6769, 2640, 14899, 10770, 56317, 52188, 64447, 60318, 39801, 35672, 47931, 43802, 
		27814, 31879, 19684, 23749, 11298, 15363, 3168, 7233, 60846, 64911, 52716, 56781, 44330, 48395, 36200, 40265, 
		32407, 28342, 24277, 20212, 15891, 11826, 7761, 3696, 65439, 61374, 57309, 53244, 48923, 44858, 40793, 36728, 
		37256, 33193, 45514, 41451, 53516, 49453, 61774, 57711, 4224, 161, 12482, 8419, 20484, 16421, 28742, 24679, 
		33721, 37784, 41979, 46042, 49981, 54044, 58239, 62302, 689, 4752, 8947, 13010, 16949, 21012, 25207, 29270, 
		46570, 42443, 38312, 34185, 62830, 58703, 54572, 50445, 13538, 9411, 5280, 1153, 29798, 25671, 21540, 17413, 
		42971, 47098, 34713, 38840, 59231, 63358, 50973, 55100, 9939, 14066, 1681, 5808, 26199, 30326, 17941, 22068, 
		55628, 51565, 63758, 59695, 39368, 35305, 47498, 43435, 22596, 18533, 30726, 26663, 6336, 2273, 14466, 10403, 
		52093, 56156, 60223, 64286, 35833, 39896, 43963, 48026, 19061, 23124, 27191, 31254, 2801, 6864, 10931, 14994, 
		64814, 60687, 56684, 52557, 48554, 44427, 40424, 36297, 31782, 27655, 23652, 19525, 15522, 11395, 7392, 3265, 
		61215, 65342, 53085, 57212, 44955, 49082, 36825, 40952, 28183, 32310, 20053, 24180, 11923, 16050, 3793, 7920
	};
#endif

// Конструктор. Инициализирует экземпляр класса и кэш-таблицу полинома
void CRC16_CCITT_1021_EvaluateTable (void)
	{
#ifdef EvaluateTable
	// Переменные
	uint value, temp, i, j;

	// Отсечка
	if (table[TableLength - 1] != 0)
		return;

	// Формирование кэш-таблицы полинома
	for (i = 0; i < TableLength; ++i)
		{
		value = 0;
		temp = i << 8;

		for (j = 0; j < 8; ++j)
			{
			if (((value ^ temp) & 0x8000) != 0)
				{
				value = (value << 1) ^ Polynomial;
				}
			else
				{
				value <<= 1;
				}

			temp <<= 1;
			}
		
		table[i] = value;
		}

	i = 0;
#endif
	}

// Метод вычисляет CRC
uint GetCRC16 (uchar *Message, uint MessageLength, uint StartPosition, uint EndPosition)
	{
	// Переменные
	crc = 0xFFFF;
	crc_i = 0;
	crc_e = MessageLength - 1;

	// Общий контроль
	if ((Message == NULL) || (MessageLength == 0))
		return 0;

	// Контроль границ
	if (StartPosition < MessageLength)
		crc_i = StartPosition;

	if (EndPosition < MessageLength)
		crc_e = EndPosition;

	// Вычисление
	for (; crc_i <= crc_e; ++crc_i)
		{
		crc = (crc << 8) ^ table[(crc >> 8) ^ (uint)Message[crc_i]];
		}

	return crc;
	}

/*
CRC implementations use tables for speed. They are not required.
Here is a short CRC32 using either the Castagnoli polynomial (same one as used by the Intel crc32 instruction),
or the Ethernet polynomial (same one as used in zip, gzip, etc.).

#include <stddef.h>
#include <stdint.h>

// CRC-32C (iSCSI) polynomial in reversed bit order
#define POLY 0x82F63B78

// CRC-32 (Ethernet, ZIP, etc.) polynomial in reversed bit order.
//#define POLY 0xEDB88320

uint32_t crc32c (uint32_t crc, const unsigned char *buf, size_t len)
{
    int k;

    crc = ~crc;
    while (len--) {
        crc ^= *buf++;
        for (k = 0; k < 8; k++)
            crc = crc & 1 ? (crc >> 1) ^ POLY : crc >> 1;
    }
    return ~crc;
}
The initial crc value should be zero. The routine can be called successively with chunks of the data to update the CRC.
You can unroll the inner loop for speed, though your compiler might do that for you anyway
*/
