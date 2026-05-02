#ifndef SRC_CONSOLE_PRODUCTMANAGEMENT_SELECTPRODUCTVIEW_H_
#define SRC_CONSOLE_PRODUCTMANAGEMENT_SELECTPRODUCTVIEW_H_

#include "Console/IView.h"

class SelectProductView : public IView
{
public:
    SelectProductView(Context context);

    std::unique_ptr<IView> Run() override;

private:
    Context m_Context;
};

#endif // SRC_CONSOLE_PRODUCTMANAGEMENT_SELECTPRODUCTVIEW_H_