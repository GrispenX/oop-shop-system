#ifndef SRC_CONSOLE_RECEIPTMANAGEMENT_RECEIPTSVIEW_H_
#define SRC_CONSOLE_RECEIPTMANAGEMENT_RECEIPTSVIEW_H_

#include "Console/IView.h"

class ReceiptsView : public IView
{
public:
    ReceiptsView(Context context);

    std::unique_ptr<IView> Run() override;

private:
    Context m_Context;
};

#endif // SRC_CONSOLE_RECEIPTMANAGEMENT_RECEIPTSVIEW_H_