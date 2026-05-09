qml/
├── Main.qml
├── components/
│   ├── ContactDelegate.qml
│   ├── FilterPanel.qml
│   └── SearchBar.qml
└── pages/
    └── ContactListPage.qml

Правило простое:
## components : 
	многоразовые части. Панель поиска, делегат для рендеринга контакта в списке, панель фильтра. Вещи, которые вы можете использовать в нескольких местах.
## pages: 
	композиции на уровне экрана. Страница, которая помещает панель поиска, панель фильтра и список контактов вместе в один согласованный экран

## CPP parts

src/
├── models/
│   ├── contact.h / .cpp
│   ├── contactlistmodel.h / .cpp
│   └── contactfilterproxymodel.h / .cpp
├── managers/
│   └── contactmanager.h / .cpp
└── utils/
    └── jsonserializer.h / .cpp

Три папки, три обязанности:

## model: ваши данные. 
	`ContactClass` – это чистый контейнер для данных. 
	`ContactListModel` является a QAbstractListModel,что обертывает QVector<Contact> и подвергает его воздействию QML через пользовательские роли. 
	`ContactFilterProxyModel` сидит сверху и обрабатывает поиск, фильтрацию и сортировку, даже не касаясь исходных данных.
## managers: 
	Ваша бизнес-логика. 
	`ContactManager` владеет моделью, координирует операции CRUD, отслеживает статистику и является единой точкой входа для QML. 
	Это класс, который вы на самом деле инициируете в своем пользовательском интерфейсе.
## utils : 
	Ваш код помощника. 
	`JsonSerializer` Обрабатывает сохранение и загрузку контактов на диск.

