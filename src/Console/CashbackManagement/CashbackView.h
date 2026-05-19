#ifndef SRC_CONSOLE_CASHBACKMANAGEMENT_CASHBACKVIEW_H_
#define SRC_CONSOLE_CASHBACKMANAGEMENT_CASHBACKVIEW_H_

#include "Console/IView.h"

class CashbackView : public IView
{
public:
    CashbackView(Context context);

    std::unique_ptr<IView> Run() override;

private:
    Context m_Context;
};

#endif // SRC_CONSOLE_CASHBACKMANAGEMENT_CASHBACKVIEW_H_