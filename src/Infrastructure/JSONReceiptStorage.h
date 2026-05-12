#ifndef SRC_INFRASTRUCTURE_JSONRECEIPTSTORAGE_H_
#define SRC_INFRASTRUCTURE_JSONRECEIPTSTORAGE_H_

#include "Core/IReceiptStorage.h"
#include <filesystem>

/**
 * JSON-backed implementation of IReceiptStorage that keeps receipts in memory
 * and associates the storage with a filesystem path for persistence.
 */

/**
 * Construct a JSONReceiptStorage bound to the given filesystem path.
 * @param path Filesystem path used to persist or identify the storage location.
 */

/**
 * Destroy the storage and release any resources held by this instance.
 */

/**
 * Store a new receipt and assign it a unique integer identifier.
 * @param receipt Receipt to store.
 * @returns The integer identifier assigned to the stored receipt.
 */

/**
 * Update an existing stored receipt with the data from the provided receipt.
 * @param receipt Receipt containing updated data (identified by its id).
 */

/**
 * Retrieve the receipt identified by the given id.
 * @param id Identifier of the desired receipt.
 * @returns The Receipt with the specified id.
 */

/**
 * Return all receipts that satisfy the provided predicate.
 * @param predicate Function applied to each stored Receipt; receipts for which
 *                  the predicate returns `true` are included in the result.
 * @returns A vector of receipts matching the predicate.
 */

/**
 * Return all receipts currently stored.
 * @returns A vector containing every stored Receipt.
 */

/**
 * Remove the receipt with the specified identifier from storage.
 * @param id Identifier of the receipt to remove.
 */
class JSONReceiptStorage : public IReceiptStorage
{
public:
    JSONReceiptStorage(std::filesystem::path path);
    ~JSONReceiptStorage();

    int AddReceipt(Receipt receipt) override;
    void UpdateReceipt(Receipt receipt) override;
    Receipt GetReceipt(int id) override;
    std::vector<Receipt> GetReceipts(std::function<bool(Receipt)> predicate) override;
    std::vector<Receipt> GetAllReceipts() override;
    void RemoveReceipt(int id) override;

private:
    std::filesystem::path m_Path;
    std::unordered_map<int, Receipt> m_Receipts;
    int m_NextID;
};

#endif // SRC_INFRASTRUCTURE_JSONRECEIPTSTORAGE_H_