#ifndef SRC_CONSOLE_PRODUCTMANAGEMENT_PRODUCTSVIEW_H_
#define SRC_CONSOLE_PRODUCTMANAGEMENT_PRODUCTSVIEW_H_

#include "Console/IView.h"

class ProductsView : public IView
{
public:
    ProductsView(Context context);

    std::unique_ptr<IView> Run() override;

private:
    Context m_Context;
};

#endif // SRC_CONSOLE_PRODUCTMANAGEMENT_PRODUCTSVIEW_H_