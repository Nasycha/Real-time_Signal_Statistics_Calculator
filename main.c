#include <stdio.h>
#include <math.h>
#include <stdint.h>


typedef struct{
    uint32_t frequency;
    uint16_t length;
    int8_t angle;
 } pulse_signal_t;

 typedef struct {
    // количество сигналов
    uint32_t count; // количество сигналов

    // Для frequency
    double mean_frequency; // среднее
    double M2_frequency; // сумма квадратов отклонений

    // Для length
    float mean_length; // среднее
    double M2_length; // сумма квадратов отклонений

    // Для angle
    float mean_angle; // среднее
    float M2_angle; // сумма квадратов отклонений
 } signal_status_t;


// Инициализация структуры signal_status_t
void init_signal_status(signal_status_t* status) { 
    status->count = 0;
    status->mean_frequency = 0.0;
    status->M2_frequency = 0.0;
    status->mean_length = 0.0;
    status->M2_length = 0.0;
    status->mean_angle = 0.0;
    status->M2_angle = 0.0;
}

// Функция для обновления статистики сигналов
void update_signal_status(signal_status_t* status, pulse_signal_t* new_signal) {
    status->count++;

    // Обновление статистики для frequency
    double delta_frequency = new_signal->frequency - status->mean_frequency; // разница между текущим значением и средним
    status->mean_frequency += delta_frequency / status->count; // обновление среднего значения
    double delta2_frequency = new_signal->frequency - status->mean_frequency;
    status->M2_frequency += delta_frequency * delta2_frequency;

    // Обновление статистики для length
    float delta_length = new_signal->length - status->mean_length;
    status->mean_length += delta_length / status->count;
    double delta2_length = new_signal->length - status->mean_length;
    status->M2_length += delta_length * delta2_length;

    // Обновление статистики для angle
    float delta_angle = new_signal->angle - status->mean_angle;
    status->mean_angle += delta_angle / status->count;
    float delta2_angle = new_signal->angle - status->mean_angle;
    status->M2_angle += delta_angle * delta2_angle;
}


double get_variance_frequency(signal_status_t* status) {
    if (status->count < 2) {
        return NAN; // Если меньше двух сигналов, дисперсия не определена
    }
    return status->M2_frequency / (status->count - 1);
}

double get_variance_length(signal_status_t* status) {
    if (status->count < 2) {
        return NAN; // Если меньше двух сигналов, дисперсия не определена
    }
    return status->M2_length / (status->count - 1);
}
float get_variance_angle(signal_status_t* status) {
    if (status->count < 2) {
        return NAN; // Если меньше двух сигналов, дисперсия не определена
    }
    return status->M2_angle / (status->count - 1);
}
// отладочный вывод
void print_signal_status(signal_status_t* status) {
    printf("Количество сигналов: %u\n", status->count);
    printf("Средняя частота (mean_freq): %f\n", status->mean_frequency);
    printf("Дисперсия частоты (var_freq): %f\n", get_variance_frequency(status));
    printf("Средняя длина (mean_len): %f\n", status->mean_length);
    printf("Дисперсия длины (var_len): %f\n", get_variance_length(status));
    printf("Средний угол (mean_angle): %f\n", status->mean_angle);
    printf("Дисперсия угла (var_angle): %f\n", get_variance_angle(status));
}


////////////////////////////// ТЕСТИРОВАНИЕ //////////////////////////////


void process_test_file(const char* csv_file) {
    FILE* f = fopen(csv_file, "r");
    if (!f) {
        fprintf(stderr, "Ошибка открытия файла: %s\n", csv_file);
        return;
    }

    signal_status_t status;
    init_signal_status(&status);

    char line[256];
    fgets(line, sizeof(line), f); // Пропускаем заголовок
    pulse_signal_t s;

    while (fgets(line, sizeof(line), f)) {
        unsigned int temp_freq;
        unsigned short temp_len;
        signed char temp_angle;

        if (sscanf(line, "%u,%hu,%hhd", &temp_freq, &temp_len, &temp_angle) == 3) {
            s.frequency = (uint32_t)temp_freq;
            s.length = (uint16_t)temp_len;
            s.angle = (int8_t)temp_angle;
            update_signal_status(&status, &s);
            }
    }

    fclose(f);

    // Печать в консоль
    printf("\n\n==== Результат для файла: %s ====\n", csv_file);
    print_signal_status(&status);

}

int main() {
    const char* test_files[] = {
        "practika_test_data/test_100_0.1.csv",
        "practika_test_data/test_100_1.csv",
        "practika_test_data/test_100_10.csv",
        "practika_test_data/test_1000_0.1.csv",
        "practika_test_data/test_1000_1.csv",
        "practika_test_data/test_1000_10.csv",
        "practika_test_data/test_10000_0.1.csv",
        "practika_test_data/test_10000_1.csv",
        "practika_test_data/test_10000_10.csv"
    };

    size_t n = sizeof(test_files) / sizeof(test_files[0]);
    for (size_t i = 0; i < n; i++) {
        process_test_file(test_files[i]);
    }

    return 0;
}

