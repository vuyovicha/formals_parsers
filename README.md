# Алгоритм Эрли

### Запуск алгоритма

Для сборки проекта после клонирования необходимо выполнить команды:

```
mkdir build
cd build
cmake ..
make
```

Для запуска программы после предыдущих комманд необходимо запустить исполняемый файл:

```
./formals
```

Необходимо следовать всем указаниям программы (в начале вводится количество правил, далее правила в требуемом формате типа "A BC", если в грамматике существует правило "A->BC"). Важно, что все нетерминалы - это заглавные буквы, и они не могут совпадать с нетерминальными символами (даже если они разных регистров). После ввода грамматики предлагается ввести количество слов для проверки на выводимость и далее сами слова.

Стартовый символ - всегда `S`. Эпсилон - всегда `e`. Нетерминал `M` всегда занят для служебного пользования! 
### Запуск тестов
Если проект был собран так, как указано в предыдущем пункте, достаточно выполнить команду:
```
./Test
```

Для просмота численного количества покрытия кода тестами необходимо установить специальную программу `lcov`. На линуксе это делается с помощью команды:
```
sudo apt-get install lcov
```
Далее необходимо выйти из директории `build` и выполнить следующие команды: 
```
cd ..
lcov --directory ./build/CMakeFiles/Test.dir/ --capture --output-file ./code_coverage.info -rc lcov_branch_coverage=1
genhtml code_coverage.info --branch-coverage --output-directory ./code_coverage_report/
```

*Процент покрытия кода тестми немного меньше, чнем требуется по ТЗ, так как в реализации присутсвуют функции, необходимые для взаимодействия с пользователем при его вводе грамматики через клавитатуру и т. д.*