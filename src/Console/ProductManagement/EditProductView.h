#ifndef SRC_CONSOLE_PRODUCTMANAGEMENT_EDITPRODUCTVIEW_H_
#define SRC_CONSOLE_PRODUCTMANAGEMENT_EDITPRODUCTVIEW_H_

#include "Console/IView.h"

class EditProductView : public IView
{
public:
    EditProductView(Context context, int product_id);

    std::unique_ptr<IView> Run() override;

private:
    Context m_Context;
    int m_ProductID;
};

#endif // SRC_CONSOLE_PRODUCTMANAGEMENT_EDITPRODUCTVIEW_H_