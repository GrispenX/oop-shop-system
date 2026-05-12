#ifndef SRC_CONSOLE_CASHBACKMANAGEMENT_EDITCUSTOMERVIEW_H_
#define SRC_CONSOLE_CASHBACKMANAGEMENT_EDITCUSTOMERVIEW_H_

#include "Console/IView.h"

/**
 * View used to edit an existing customer within the cashback management console.
 */

/**
 * Construct an EditCustomerView for a specific customer.
 * @param context Application context providing access to services and state required by the view.
 * @param customer_id Identifier of the customer to be edited.
 */

/**
 * Execute the view's interaction flow and produce the next view.
 * @returns A unique_ptr to the next IView to display, or `nullptr` to terminate the flow.
 */
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