get-m ([id] - курс за id) або ([all] - список всіх курсів)
get-s ([courceid] all - список всіх завдань за курсом) або ([courceid] id [id] - завдання за курсом та id)
del-m ([id] - видалити курс за id)
del-s ([courceid] id [id] - видалити завдання за курсом та id)
update-m id [id] (name [name] - оновити поле name курсу за id) або (description [description] - оновити поле description курсу за id)
update-s [courceid] id [id] (name [name] - оновити поле name завдання id курсу courceid) або (content [content] - оновити поле content завдання id курсу courceid)
insert-m name [name] description [description] - додати курс з полями name та description
insert-s courceid [courceid] name [name] content [content] - додати завдання до курсу courceid з полями name та content
calc-m - загальна кількість курсів
calc-s (total - загальна кількість завдань) або (id [id] - кількість завдань курсу id)
ut-m - отримати вміст таблиці курсів зі службовими полями
ut-s - отримати вміст таблиці завдань зі службовими полями