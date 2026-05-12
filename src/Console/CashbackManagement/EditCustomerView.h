#ifndef SRC_CONSOLE_CASHBACKMANAGEMENT_EDITCUSTOMERVIEW_H_
#define SRC_CONSOLE_CASHBACKMANAGEMENT_EDITCUSTOMERVIEW_H_

#include "Console/IView.h"

class EditCustomerView : public IView
{
public:
    EditCustomerView(Context context, int customer_id);

    std::unique_ptr<IView> Run() override;

private:
    Context m_Context;
    int m_CustomerID;
};

#endif // SRC_CONSOLE_CASHBACKMANAGEMENT_EDITCUSTOMERVIEW_H_