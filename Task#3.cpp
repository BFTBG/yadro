#include <iostream>
#include <vector>
#include <random>
#include <fstream>

class BinarySymmetricChannel {
public:
    BinarySymmetricChannel(double errorProbability)
        : errorProbability(errorProbability), generator(std::random_device{}()), distribution(0.0, 1.0) {}

    std::vector<int> transmit(const std::vector<int>& input) {
        std::vector<int> output;
        for (int bit : input) {
            if (distribution(generator) < errorProbability) {
                output.push_back(1 - bit); // Инвертируем бит с заданной вероятностью
            } else {
                output.push_back(bit); // Передаем бит без изменений
            }
        }
        return output;
    }

private:
    double errorProbability;
    std::mt19937 generator; // Генератор случайных чисел
    std::uniform_real_distribution<> distribution; // Распределение от 0 до 1
};

int main() {
    const int numTrials = 1000; // Количество испытаний
    const int numBits = 100; // Количество бит для передачи
    std::vector<double> channelErrorProbabilities;
    std::vector<double> decoderErrorProbabilities;

    for (double channelErrorProbability = 0.0; channelErrorProbability <= 1.0; channelErrorProbability += 0.1) {
        BinarySymmetricChannel bsc(channelErrorProbability);
        int decoderErrors = 0;

        for (int trial = 0; trial < numTrials; ++trial) {
            // Генерируем случайный набор бит
            std::vector<int> input(numBits);
            for (int &bit : input) {
                bit = rand() % 2; // Генерация случайного бита
            }

            // Передаем данные через двоичный симметричный канал
            std::vector<int> output = bsc.transmit(input);

            // Подсчет ошибок декодера
            for (size_t i = 0; i < input.size(); ++i) {
                if (input[i] != output[i]) {
                    decoderErrors++; // Увеличиваем счетчик ошибок декодера
                }
            }
        }

        // Вероятность ошибки на выходе декодера
        double decoderErrorProbability = static_cast<double>(decoderErrors) / (numTrials * numBits);
        channelErrorProbabilities.push_back(channelErrorProbability);
        decoderErrorProbabilities.push_back(decoderErrorProbability);
    }

    // Запись данных в файл
    std::ofstream file("error_probabilities.dat");
    for (size_t i = 0; i < channelErrorProbabilities.size(); ++i) {
        file << channelErrorProbabilities[i] << " " << decoderErrorProbabilities[i] << "\n";
    }
    file.close();

    // Команды для Gnuplot 
    // Названия осей даны на английском так как Gnuplotyt не воспринимает русский язык
    std::ofstream gnuplotScript("plot.gnuplot");
    gnuplotScript << "set title 'Dependence of the error probability at the decoder output on the error probability in the channel'\n";
    gnuplotScript << "set xlabel 'Channel Error Probability'\n";
    gnuplotScript << "set ylabel 'Decoder output error probability'\n";
    gnuplotScript << "plot 'error_probabilities.dat' with linespoints title 'ERROR'\n";
    gnuplotScript.close();
    
    // Запуск gnuplot с созданным скриптом
    system("gnuplot -p plot.gnuplot");
    
    return 0;
}