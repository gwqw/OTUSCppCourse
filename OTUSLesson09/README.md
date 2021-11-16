# OTUSLesson09
# print_ip

## Условие

Реализовать функцию печати условного ip-адреса.
Адрес может быть представлен в виде произвольного целочисленного типа. Представление не зависит от знака типа. Выводить побайтово начиная со старшего с символом . в качестве разделителя.
Адрес может быть представлен в виде строки. Выводится как есть.
Адрес может быть представлен в виде контейнеров std::list, std::vector. Выводится содержимое контейнера поэлементно и разделяется . (одним символом точка).
Опционально адрес может быть представлен в виде std::tuple при условии, что все типы одинаковы.
Выводится содержимое поэлементно и разделяется . (одним символом точка).
Прикладной код должен содержать следующие вызовы:
	• Печать адреса как char(-1)
	• Печать адреса как short(0)
	• Печать адреса как int(2130706433)
	• Печать адреса как long(8875824491850138409)
	• Печать адреса как std::string
	• Печать адреса как std::vector
	• Печать адреса как std::list
	• Опционально. Печать адреса как std::tuple
Добавить в .travis.yml на этапе сборки вызов doxygen и публикацию html-версии документации на githubpages.
Подробное описание на странице:
https://docs.travis-ci.com/user/deployment/pages/
Включить в репозиторий файл Doxyfile с включенными опциями HAVE_DOT и EXTRACT_ALL.

## Самоконтроль

​	• функция печати должна быть одной шаблонной функцией с частичными специализациями
​	• специализация для целочисленного представления должна быть единственная
​	• специализация для контейнеров должна быть единственная
​	• не должно быть реализации типа “если не совпало с иными - значит это контейнер”
​	• опциональная реализация для std::tuple должна приводить к ошибке в случае отличающихся типов
​	• найдите самый простой способ сделать для печати std::string
​	• пакет print_ip содержащий бинарный файл print_ip опубликован на bintray
​	• отправлена на проверку ссылка на страницу репозитория
​	• отправлена ссылка на страницу документации

## Проверка

Задание считается выполненным успешно если, после просмотра кода, установки пакета и запуска бинарного
файла командой:
`$ print_ip`
будут выведены адреса

```
255
0.0
127.0.0.1
123.45.67.89.101.112.131.41
```


вслед за которыми будут выведены адреса из строки, контейнеров и опционально из кортежа.


