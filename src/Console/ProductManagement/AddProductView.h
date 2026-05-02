#ifndef SRC_CONSOLE_PRODUCTMANAGEMENT_ADDPRODUCTVIEW_H_
#define SRC_CONSOLE_PRODUCTMANAGEMENT_ADDPRODUCTVIEW_H_

#include "Console/IView.h"

class AddProductView : public IView
{
public:
    AddProductView(Context context);

    std::unique_ptr<IView> Run() override;

private:
    Context m_Context;
};

#endif // SRC_CONSOLE_PRODUCTMANAGEMENT_ADDPRODUCTVIEW_H_