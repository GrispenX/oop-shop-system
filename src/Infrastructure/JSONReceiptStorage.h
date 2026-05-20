#ifndef SRC_INFRASTRUCTURE_JSONRECEIPTSTORAGE_H_
#define SRC_INFRASTRUCTURE_JSONRECEIPTSTORAGE_H_

#include "Core/IReceiptStorage.h"
#include <filesystem>

class JSONReceiptStorage : public IReceiptStorage
{
public:
    JSONReceiptStorage(std::filesystem::path path);
    ~JSONReceiptStorage();

    int AddReceipt(Receipt receipt) override;
    void UpdateReceipt(Receipt receipt) override;
    std::optional<Receipt> GetReceipt(int id) override;
    std::vector<Receipt> GetReceipts(std::function<bool(Receipt)> predicate) override;
    std::vector<Receipt> GetAllReceipts() override;
    void RemoveReceipt(int id) override;

private:
    std::filesystem::path m_Path;
    std::unordered_map<int, Receipt> m_Receipts;
    int m_NextID;
};

#endif // SRC_INFRASTRUCTURE_JSONRECEIPTSTORAGE_H_