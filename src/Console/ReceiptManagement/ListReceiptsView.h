#ifndef SRC_CONSOLE_RECEIPTMANAGEMENT_LISTRECEIPTSVIEW_H_
#define SRC_CONSOLE_RECEIPTMANAGEMENT_LISTRECEIPTSVIEW_H_

#include "Console/IView.h"

class ListReceiptsView : public IView
{
public:
    ListReceiptsView(Context context);

    std::unique_ptr<IView> Run() override;

private:
    Context m_Context;
};

#endif // SRC_CONSOLE_RECEIPTMANAGEMENT_LISTRECEIPTSVIEW_H_