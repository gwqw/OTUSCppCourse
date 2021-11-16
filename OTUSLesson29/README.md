# kkmeans (OTUSLesson29)
solution to otus c++ homework 15 (lesson 29)

## Условие

Цель задания кластеризация и визуализация результата.
На подготовительном этапе следует подключить библиотеку dlib к своему сборочному окружению. Библиотека находится в пакете `libdlib-dev`. Для работы потребуется библиотека линейной алгебры, которая в свою очередь находится в пакете `libopenblas-dev`.
Установка дополнительного репозитория рассматривалась на первом занятии:

```sh
echo "deb http://archive.ubuntu.com/ubuntu xenial main universe" \
| sudo tee -a /etc/apt/sources.list
```
В качестве основы необходимо взять пример http://dlib.net/kkmeans_ex.cpp.html и собрать из него действующее приложение.
На основе этого примера необходимо разработать приложение, принимающее
на вход строки вида

```sh
x1;y1\n
x2;y2\n
x3;y3\n
```
где пары `xn` и `yn` - это координаты точек в пространстве.
приложения должны формироваться строки вида:

```sh
x1;y1;cluster1\n
x2;y2;cluster2\n
x3;y3;cluster3\n
```
где пары `xn` и `yn` - идентичны входным, `clustern` - номер кластера, к которому относится соответствующая пара.
Подготовить набор входных данных идентичных примеру (учитывая случайность величин один из возможных вариантов).
Подготовить набор входных данных “звездное небо” состоящее из 200 случайных точек координаты которых в диапазоне от -100 до 100 включительно.
Наборы данных должны быть включены в состав пакета.
Приложение должно запускаться командой:

```sh
cat kkmeans_ex.txt | kkmeans n
```
где `n` - количество кластеров.
Приложить результат визуализации “звездного неба” пузырьковой диаграммой.
Опционально - предоставить последовательность действий для быстрой визуализации из командной строки (генерация изображения).

## Самоконтроль

- пакет `kkmeans` содержащий исполняемый файл `kkmeans` опубликован на `bintray`
- отправлена на проверку ссылка на страницу репозитория

## Проверка

Задание считается выполненным успешно, если после установки пакета и запуска с тестовыми данными удается визуализировать результат пузырьковой диаграммой.

## Как использовать

**points_generator.py** -- генерит набор точек (x;y) в стандартный output. Параметр командной строки -- тип генерации:
1 --  3 окружности
2 -- точки в квадрате [-100;100]
3 -- 3 окружности + точки в квадрате
**kkmeans** -- проводит кластеризацию точек, поступающих в стандартный ввод (x;y) и выводит в стандартный вывод точки с номером кластера: x;y;c

```sh
kkmeans <n> [<kernel_type> [<gamma> <coef> <degree> <tolerance> <max_dict_size>]]
```
`n` -- число кластеров
`kernel_type` -- тип ядра: 0 - radial (default), 1 - polynomial, 2 - sigmoid, 3 - linear, 4 - histogram
`gamma coef degree` -- какие-то параметры ядра (default: 0.1 0 1)
`tolerance` -- погрешность
`max_dict_size` -- максимальный размер словаря
**plot_results.py** -- отображает результаты кластеризации в виде диаграммы, разные кластеры -- разные цвета
**Example**: 

```sh
./points_generator.py 2 | ./kkmeans 4 1 0.0001 | ./plot_results.py
```
