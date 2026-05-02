#ifndef SRC_CONSOLE_PRODUCTMANAGEMENT_LISTPRODUCTSVIEW_H_
#define SRC_CONSOLE_PRODUCTMANAGEMENT_LISTPRODUCTSVIEW_H_

#include "Console/IView.h"

class ListProductsView : public IView
{
public:
    ListProductsView(Context context);

    std::unique_ptr<IView> Run() override;

private:
    Context m_Context;
};

#endif // SRC_CONSOLE_PRODUCTMANAGEMENT_LISTPRODUCTSVIEW_H_