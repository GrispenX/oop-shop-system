#ifndef SRC_CONSOLE_RECEIPTMANAGEMENT_RECEIPTDETAILSVIEW_H_
#define SRC_CONSOLE_RECEIPTMANAGEMENT_RECEIPTDETAILSVIEW_H_

#include "Console/IView.h"

class ReceiptDetailsView : public IView
{
public:
    ReceiptDetailsView(Context context, int receipt_id);

    std::unique_ptr<IView> Run() override;

private:
    Context m_Context;
    int m_ReceiptID;
};

#endif // SRC_CONSOLE_RECEIPTMANAGEMENT_RECEIPTDETAILSVIEW_H_