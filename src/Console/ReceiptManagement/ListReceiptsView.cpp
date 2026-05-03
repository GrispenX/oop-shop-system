#include "Console/ReceiptManagement/ListReceiptsView.h"
#include "Console/ReceiptManagement/ReceiptsView.h"
#include <iostream>
#include <iomanip>

ListReceiptsView::ListReceiptsView(Context context) :
    m_Context(context)
{

}

std::unique_ptr<IView> ListReceiptsView::Run()
{
    std::cout << "===== Receipts list =====\n";

    std::cout << std::setw(4) << "ID" << " ";
    std::cout << std::setw(10) << "Status" << " ";
    std::cout << std::setw(20) << "Timestamp" << " ";
    std::cout << std::setw(10) << "Total" << "\n";
    for(int i = 0; i < 4 + 10 + 20 + 10 + 3; i++) std::cout << "-";
    std::cout << "\n";

    for(auto& r : m_Context.receipt_service->GetAllReceipts())
    {
        std::time_t timestamp = r.GetTimestamp();
        std::tm* local_timestamp = std::localtime(&timestamp);
        char str_time[20];
        std::strftime(str_time, sizeof(str_time), "%Y-%m-%d %T", local_timestamp);

        std::cout << std::setw(4) << r.GetID() << "|";
        std::cout << std::setw(10) << r.GetStatus() << "|"; 
        std::cout << std::setw(20) << str_time << "|";
        std::cout << std::setw(10) << r.CalcTotal() << "$\n";
    }

    std::cout << "\n";

    return std::make_unique<ReceiptsView>(m_Context);
}