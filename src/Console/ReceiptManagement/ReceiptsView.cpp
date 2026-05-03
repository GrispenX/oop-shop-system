#include "Console/ReceiptManagement/ReceiptsView.h"
#include "Console/ReceiptManagement/ListReceiptsView.h"
#include "Console/ReceiptManagement/SelectReceiptView.h"
#include "Console/ReceiptManagement/ReceiptDetailsView.h"
#include "Console/MainView.h"
#include "Console/ReadWithValidation.h"
#include <iostream>

ReceiptsView::ReceiptsView(Context context) :
    m_Context(context)
{

}

std::unique_ptr<IView> ReceiptsView::Run()
{
    std::cout << "===== Receipts =====\n";
    std::cout << "  1. List receipts\n";
    std::cout << "  2. New receipt\n";
    std::cout << "  3. View receipt details\n";
    std::cout << "  4. Back\n";
    std::cout << "\n";

    int choice = ReadWithValidation<int>("Choice", [](int input) { return input >= 1 && input <= 4; });

    switch (choice)
    {
    case 1:
        std::cout << "\n";
        return std::make_unique<ListReceiptsView>(m_Context);
        break;

    case 2:
        std::cout << "\n";
        return std::make_unique<ReceiptDetailsView>(m_Context, m_Context.receipt_service->StartNewReceipt());
        break;

    case 3:
        return std::make_unique<SelectReceiptView>(m_Context);
        break;
    
    default:
        std::cout << "\n";
        return std::make_unique<MainView>(m_Context);
        break;
    }
}