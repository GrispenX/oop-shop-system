#ifndef SRC_CONSOLE_CASHBACKMANAGEMENT_CASHBACKVIEW_H_
#define SRC_CONSOLE_CASHBACKMANAGEMENT_CASHBACKVIEW_H_

#include "Console/IView.h"

/**
 * Console view for cashback management.
 *
 * Presents the cashback management interaction flow and produces the next view to display.
 */
 
/**
 * Initialize the cashback view with the given application context.
 *
 * @param context Application context containing services and state required by the view.
 */
 
/**
 * Execute the cashback view's main interaction loop and produce the next view.
 *
 * @returns A unique pointer to the next IView to display; may be null to indicate application exit.
 */
class CashbackView : public IView
{
public:
    CashbackView(Context context);

    std::unique_ptr<IView> Run() override;

private:
    Context m_Context;
};

#endif // SRC_CONSOLE_CASHBACKMANAGEMENT_CASHBACKVIEW_H_