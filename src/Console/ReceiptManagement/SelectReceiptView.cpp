#include "Console/ReceiptManagement/SelectReceiptView.h"
#include "Console/ReceiptManagement/ReceiptDetailsView.h"
#include "Console/SafeRead.h"
#include "Console/TerminalStyle.h"
#include <iostream>

SelectReceiptView::SelectReceiptView(Context context) :
    m_Context(context)
{

}

std::unique_ptr<IView> SelectReceiptView::Run()
{
    int id;
    while (!SafeRead("Receipt ID", id))
    {
        TerminalStyle::PrintError("ID should be an integer");
    }
    

    std::optional<Receipt> receipt = m_Context.receipt_service->GetReceipt(id);

    if(receipt.has_value())
    {
        std::cout << "\n";
        return std::make_unique<ReceiptDetailsView>(m_Context, id);
    }
    else
    {
        TerminalStyle::PrintError("Receipt not found");
        return std::make_unique<SelectReceiptView>(m_Context);
    }
}