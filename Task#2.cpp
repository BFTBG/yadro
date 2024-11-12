#include <iostream>
#include <vector>
#include <random>

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
    
    // Задаем вероятность ошибки
    std::cout << "Input error probability:"<< std::endl;
    double errorProbability;
    std::cin >> errorProbability;
    BinarySymmetricChannel bsc(errorProbability);

    // Исходный набор бит
    std::vector<int> input = {0, 1, 0, 1, 1, 0};

    // Передаем данные через канал
    std::vector<int> output = bsc.transmit(input);

    // Вывод результатов
    std::cout << "Input:  ";
    for (int bit : input) {
        std::cout << bit << " ";
    }
    std::cout << "\nOutput: ";
    for (int bit : output) {
        std::cout << bit << " ";
    }
    std::cout << std::endl;
    system("pause");
    return 0;
}