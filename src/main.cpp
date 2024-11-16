#include <iostream>

#include "FileManager.hpp"
#include "Sorter.hpp"

// zapisujemy i odczytujemy blokami (wczytać cały blok do bufora)
// co jeżeli seria będzie miała koniec w następnym bloku? (może wczytać następny blok i dopiero wtedy zakończyć)?
// trzy tasimy
// w buforze do zapisu wartości wypływają bąbelkowo
// czytamy i zapisujemy CAŁY blok lub do wyczerpania się danych
// rozmiar bloku 4096 bajtów ; rozmiar rekordu 30 bajtów ~ 136 rekordów na blok + 16 bajtów nie używane
// aby zapisać blok zrobić zwykły append
// nalezy podać numer bloku lub pamietać gdzie skończyliśmy czytać
// czyścić taśmy dopiero gdy dane się wyczerpały
// seria - ciąg posortowanych (niemalejących) elementów (napisy kolejność leksykograficzna)
// faza - dystrybucja i scalanie
// dystrybucja jest w przypadku polifazowgo tylko jedna
// gdyby ilość serii była nie idealna to "dodać" pustę serie do jednego pliku (dłuższego)
// x+d + y = fib(n)
// serie na jednej taśmie mogą się sklejać, rozwiązać to można pamietając ostatnie klucze starej serii
// czytać rekord po rekordzie i ładować do bufora wyjściowego
// taśma - plik dyskowy (.txt, .bin idt.)
// t1     t2     t3
// 13     8      0
// 5      0      8
// 0      5      3
// 3      2      0
// 1      0      2
// 0      1      1
// 1      0      0
// znaki od 33-126
// rand % 94 + 33;

// to co w mainie będzie do App przenieść
// FileManager - odczyt i zapis na taśmy, tworzenie taśm, generacja przykładów
// Sorter - sortowanie taśm, dostaje buffor / strukturę danych zawierającą blok + informacje o końcach serii
// App - obsługa wejścia od użytkownika

int main()
{
    // FileManager::GetInstance().GenerateEmptyFile("./tapes/tape2.txt");
    // FileManager::GetInstance().GenerateEmptyFile("./tapes/tape1.txt");
    //FileManager::GetInstance().GenerateFile(100000, "./tapes/test5.txt");
    //  FileManager::GetInstance().ReadFile("./tapes/tape3.txt");
    // FileManager::GetInstance().ReadBlockFromFile("./tapes/test.txt", 1);
    // FileManager::GetInstance().ReadBlockFromFile("./tapes/tape3.txt", 1);

    // FileManager::GetInstance().ReadBlockFromFile("./tapes/tape3.txt", 2, tape1Buffer);
    // FileManager::GetInstance().WriteBlockToFile("./tapes/test.txt", tape1Buffer);
    //  Sorter::GetInstance().Fib(10);
    // Sorter::GetInstance().Sort("./tapes/tape3.txt");
    Sorter::GetInstance().Sort("./tapes/test3.txt");
    // Sorter::GetInstance().Sort("./tapes/tape1.txt");

    return 0;
}