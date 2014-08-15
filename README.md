semantic-laws-structure
=======================
Это проект бакалаврской работы. Ниже пошаговое описание с отсылками к скриптам.

Парсинг (препроцессинг) данных
=================================
*data1/parsewikisource.pl*
Выбирает из дампа Викитеки кодексы, для каждого их них создаёт отдельную папку в data1/res1/, пишет их постатейно в каждый файл в папку кодекса. Иерархия до статьи (включительно) сохраняется в файл codes_structure.txt в формате:

code;redaction;some_parts{0,4};chapter;subparts{0,2);article \tab\tab filepath <br>
(кодекс;редакция;разделы «старше» главы (от 1 до 4);глава;разделы меньше главы (1-2); статья двойной\_таб относительный\_путь\_к\_файлу (кодекс/имяфайла))
На вход: дамп Викитеки.

Грамматики и вспомогательные файлы
==================================
*references.cxx*
Грамматика для ссылок. Дополнительно использует файлы: law\_names.txt и parts\_of_law.txt с названиями законов и их частей.

*np.cxx*
Грамматика для именных групп, импортирует грамматику со ссылками. Дополнительно использует badnpstart.txt для отсеивания слов, с которых нежелательно, чтоб начиналась группа.

*definitions.cxx*
Грамматика для извлечения определений, импортирует грамматику для ИГ. Дополнительно использует badnpstart.txt

*legal.cxx*
Грамматика для извлечения (структурирования) нормативных фактов, импортирует грамматику определений. Дополнительно использует: badpred.txt (для выбора тех предикатов, что не используются в определениях), badnpstart.txt и time_words.txt (список слов, относящихся к времени).

Конфигурационные файлы и запуск
==================================
*facttypes.proto*
файл с описанием фактов и их полей

*kwtypes.proto*
вспомогательный файл для определения названия статей

*maindic.gzt*
Главный словарь, в котором описаны подключаемые газеттиры и грамматики.

*config_legal.proto*
Главный конфигурационный файлв, в котором описано: откуда читать, куда писать. какие факты интерпретировать и какие статьи для этого запускать.

*run_toma.sh*
Запускает Томиту с config\_legal.proto на папках с кодексами, на выход отдаёт папку с размеченными файлами "имя_кодекса.txt".
На вход: директория с текстовыми файлами (data/res), директория для результатов (txtres).

Тесты
===================================
Для проверки, не сломали ли что-нибудь последние изменения в грамматиках.
*config_test.proto*
Конфигурационный файл для тестов (должен подключать те же грамматики и словари, что и config_legal.proto). Читает из stdin, пишет в stdout.

*run_test.sh*
скрипт, проверяющий текущее состояние грамматики относительно тестовых файлов (в test_set1). Все именя жёстко зафиксированы.

*make_goldtest.sh*
скрипт генерирует новые разборы для тестовых файлов из конкретной директории (test_set1). Нужен при изменении схемы, которое считается более правильным.

Директория test_set1 - содержит txt файлы из одного предложения и их разбор (name.txt -> name.facts). Отобраны вручную.

Оценка
====================================
Директория с файлами - /trunk/evaldir
Далее по подпапкам:
data/ - папки по кодексам с отобранными статьями.
deftoma/ - папка с результатами работы грамматики определений на кодексах из data, каждой папке кодекса из data соответствует файл "имя_кодекса.txt"
legalres/ - папка с результатами работы грамматики для извлечения содержательных компонент норм
golddef/ - папка с определениями из кодексов, доразмечены вручную
goldlegal/ - папка с содержательными компонентами, доразмечены вручную

Скрипты:
*choose_for_eval.sh*<br>
Случайным образом отбирает по 5 статей из целевых кодексов и по 1 из остальных.
На вход: <br>
  * первым параметром папка с кодексами (data/res),<br>
  * вторым - папка, куда кладутся отобранные статьи (evaldir/data).<br>

*count_eval.sh*<br>
Подсчитывает количество полей фактов определённого типа отдельно по целевым и остальным кодексам.
На вход: <br>
  * первым параметром - папка с размеченными кодексами,<br>
  * вторым - поле факта (Subject, SubjectKey...)<br>

Порядок запуска:<br>
  * choose_for_eval.sh (data/res) (evaldir/data)<br>
  * run_toma.sh (evaldir/data) (evaldir/toma)<br>
  * cp (evaldir/toma) (evaldir/goldtoma) | _разметка руками_<br>
  * count_eval.sh (evaldir/toma) (Subject..)<br>
  * count_eval.sh (evaldir/goldtoma) (Subject..)<br>
