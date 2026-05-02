#ifndef SRC_CONSOLE_MAINVIEW_H_
#define SRC_CONSOLE_MAINVIEW_H_

#include "Console/IView.h"

class MainView : public IView
{
public:
    MainView(Context context);

    std::unique_ptr<IView> Run() override;

private:
    Context m_Context;
};

#endif // SRC_CONSOLE_MAINVIEW_H_