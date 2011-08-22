#ifndef VARIANTUTIL_H
#define	VARIANTUTIL_H

#include <QVariant>

template <class T> class VariantUtil
{
public:
    static T* toPointer(QVariant variant)
    {
        return  (T *) variant.value<void *>();
    }

    static QVariant toQVariant(T* pointer)
    {
        return qVariantFromValue((void *) pointer);
    }

private:
    /**
     * Hidden constructor.
     * Utility pattern.
     */
    VariantUtil() {};
};

#endif	/* VARIANTUTIL_H */

