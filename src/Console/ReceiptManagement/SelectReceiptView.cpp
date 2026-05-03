#include "Console/ReceiptManagement/SelectReceiptView.h"
#include "Console/ReceiptManagement/ReceiptDetailsView.h"
#include <iostream>

SelectReceiptView::SelectReceiptView(Context context) :
    m_Context(context)
{

}

std::unique_ptr<IView> SelectReceiptView::Run()
{
    int id;
    std::cout << "Receipt ID: ";
    std::cin >> id;

    std::optional<Receipt> receipt = m_Context.receipt_service->GetReceipt(id);

    if(receipt.has_value())
    {
        std::cout << "\n";
        return std::make_unique<ReceiptDetailsView>(m_Context, id);
    }
    else
    {
        std::cout << "\033[1;31mReceipt not found! Try again\033[0m\n";
        return std::make_unique<SelectReceiptView>(m_Context);
    }
}