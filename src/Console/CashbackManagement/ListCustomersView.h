#ifndef SRC_CONSOLE_CASHBACKMANAGEMENT_LISTCUSTOMERSVIEW_H_
#define SRC_CONSOLE_CASHBACKMANAGEMENT_LISTCUSTOMERSVIEW_H_

#include "Console/IView.h"

/**
 * View that lists customers and allows navigation to subsequent console views.
 */
 
/**
 * Construct the ListCustomersView with the given execution context.
 * @param context Runtime context containing services and state required by the view.
 */

/**
 * Execute the view's interaction loop and determine the next view.
 * @returns `std::unique_ptr<IView>` owning the next view/state to transition to, or `nullptr` to terminate the flow.
 */
class ListCustomersView : public IView
{
public:
    ListCustomersView(Context context);

    std::unique_ptr<IView> Run() override;

private:
    Context m_Context;
};

#endif // SRC_CONSOLE_CASHBACKMANAGEMENT_LISTCUSTOMERSVIEW_H_