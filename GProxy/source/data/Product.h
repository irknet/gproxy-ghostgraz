#ifndef PRODUCT_H
#define	PRODUCT_H

#include <QObject>
#include <QMetaType>

class Product : public QObject
{
    Q_OBJECT

public:
    Product();
    Product(const QString& productCode);
    Product(const Product& orig);
    Product& operator= (const Product& orig);
    virtual ~Product();

    QString getName() const;
    QString getProductCode() const;

private:
    QString productCode;
    // TODO QIcon icon;

    void setProductCode(const QString& productCode);
};

Q_DECLARE_METATYPE(Product)

#endif	/* PRODUCT_H */

