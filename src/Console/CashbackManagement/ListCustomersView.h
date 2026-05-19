#ifndef SRC_CONSOLE_CASHBACKMANAGEMENT_LISTCUSTOMERSVIEW_H_
#define SRC_CONSOLE_CASHBACKMANAGEMENT_LISTCUSTOMERSVIEW_H_

#include "Console/IView.h"

class ListCustomersView : public IView
{
public:
    ListCustomersView(Context context);

    std::unique_ptr<IView> Run() override;

private:
    Context m_Context;
};

#endif // SRC_CONSOLE_CASHBACKMANAGEMENT_LISTCUSTOMERSVIEW_H_