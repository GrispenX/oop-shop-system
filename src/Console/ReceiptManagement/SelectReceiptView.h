#ifndef SRC_CONSOLE_RECEIPTMANAGEMENT_SELECTRECEIPTVIEW_H_
#define SRC_CONSOLE_RECEIPTMANAGEMENT_SELECTRECEIPTVIEW_H_

#include "Console/IView.h"

class SelectReceiptView : public IView
{
public:
    SelectReceiptView(Context context);

    std::unique_ptr<IView> Run() override;

private:
    Context m_Context;
};

#endif // SRC_CONSOLE_RECEIPTMANAGEMENT_SELECTRECEIPTVIEW_H_