# Iteration 2 - ATP Management System

Повністю готове керування товарами та чеками. Можна створювати та редагувати товари, керувати їх наявністю, встановлювати на них знижки (в результаті ітерації 1). При додаванні товару до чеку кількість на складі автоматично зменшується, при відміні чеку збільшується.

В процесі цієї ітерації додалися інтерфейси для сервісів (IProductService, IReceiptService). Для реалізації системи інвентарю з'явився новий репозиторій IInventoryStorage, він додався до ProductService як залежність. ReceiptService тепер залежить від IProductService для зміни наявності при продажу.

По ходу реалізації системи кешбеку з'явились нові кор сутності та інтерфейси Customer, ICashbackStrategy, ICustomerStorage, до Receipt додалися поля m_CustomerID, m_UsedCashback. Додався новий сервіс CashbackService, від якого став залежати ReceiptService.

InMem репозиторії змінились JSON Storage'ами. Додато JSONProductSerializer та JSONReceiptSerializer, JSONCustomerSerializer.