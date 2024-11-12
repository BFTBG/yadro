#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>

class ConvolutionalEncoder {
public:
    ConvolutionalEncoder(int k, int n) : k(k), n(n) {
        // Инициализация генераторов
        // Пример: два полинома генерации
        generator = { 0b111, 0b101 }; // Пример генераторов
    }

    std::vector<int> encode(const std::vector<int>& input) {
        std::vector<int> output;
        int state = 0;
        
        for (int bit : input) {
            state <<= 1; 
            state |= bit;
            // Формирование выходных битов по генераторам
            for (int g : generator) {
                output.push_back((state & g) ? 1 : 0);
            }
            state &= (1 << (k - 1)) - 1; // Обрезаем лишние биты
        }
        
        return output;
    }
    
private:
    int k; // Количество входных бит
    int n; // Количество выходных бит
    std::vector<int> generator; // Генераторы
};

class ViterbiDecoder {
public:
    ViterbiDecoder(int k, int n) : k(k), n(n) {}

    std::vector<int> decode(const std::vector<int>& received) {
        int numStates = 1 << (k - 1);
        std::vector<std::vector<double>> metrics(numStates, std::vector<double>(received.size() + 1, std::numeric_limits<double>::infinity()));
        std::vector<std::vector<int>> paths(numStates, std::vector<int>(received.size() + 1, -1));
        
        metrics[0][0] = 0; // Начальное состояние
        
        for (size_t i = 0; i < received.size(); ++i) {
            for (int state = 0; state < numStates; ++state) {
                for (int input = 0; input < 2; ++input) {
                    int nextState = (state << 1 | input) & (numStates - 1);
                    double cost = calculateCost(input, i, received);
                    double newMetric = metrics[state][i] + cost;

                    if (newMetric < metrics[nextState][i + 1]) {
                        metrics[nextState][i + 1] = newMetric;
                        paths[nextState][i + 1] = state;
                    }
                }
            }
        }

        // Найдите лучшее состояние в конце
        int endState = 0; // Подходящее состояние на выходе
        for (int state = 0; state < numStates; ++state) {
            if (metrics[state].back() < metrics[endState].back()) {
                endState = state;
            }
        }

        // Восстановление входных битов
        std::vector<int> decoded;
        for (size_t i = received.size(); i > 0; --i) {
            int prevState = paths[endState][i];
            int inputBit = (prevState & 1);
            decoded.push_back(inputBit);
            endState = prevState;
        }
        std::reverse(decoded.begin(), decoded.end());
        
        return decoded;
    }

private:
    int k; // Количество входных бит
    int n; // Количество выходных бит

    double calculateCost(int input, size_t index, const std::vector<int>& received) {
        // Пример: использование простого метрика
        return (received[index] == input) ? 0 : 1;
    }
};

int main() {
    // Пример использования
    std::cout << "Input Number of input bits and output bits: " << std::endl;
    int k; // Количество входных бит
    int n; // Количество выходных бит
    std::cin>>k;
    std::cin>>n;

    ConvolutionalEncoder encoder(k, n);

    std::vector<int> input = { 1, 0, 1};
    std::vector<int> encoded = encoder.encode(input);
    
    std::cout << "Encoded: ";
    for (int bit : encoded) {
        std::cout << bit;
    }
    std::cout << std::endl;

    ViterbiDecoder decoder(k, n);
    std::vector<int> decoded = decoder.decode(encoded);
    
    std::cout << "Decoded: ";
    for (int bit : decoded) {
        std::cout << bit;
    }
    std::cout << std::endl;
    system("pause");

    return 0;
}