#ifndef WIZSMARTPOINTER_H_INCLUDED
#define WIZSMARTPOINTER_H_INCLUDED


// do not make SmartPtr of SmartPtr of T(template)
// do not make Cycle of SmartPtrs


namespace wiz {

    template < class T >
    class SmartPtr
    {
    private:
        class Inner {
        public:
            T* ptr = nullptr;
            int count = 0;
        };
    private:
        Inner* inner = nullptr;
        int option;
    private:
        void quit()
        {
            if (inner) {
                inner->count--;
                if (0 == inner->count) {
                    delete inner;
                }
                inner = nullptr;
            }
        }

        void enter(const SmartPtr<T>& sp)
        {
            //wizard::assertNotnullptr( sp );
            //wizard::assertnullptr( this->ptr );

            if (nullptr == sp.inner)
            {
                this->quit();
            }
            else if (nullptr == this->inner)
            {
                this->inner = sp.inner;
                sp.inner->count++;
                this->option = sp.option; ///
            }
        }

        void initFromOther(const SmartPtr<T>& sp)
        {
            //wizard::assertNotEquals( this, &sp );
           // if( this == sp.getThis() ) { return; } 

            // delete or quit
            if (this->inner)
            {
                if (isOnlyOne()) //
                {
                    remove();
                }
                else
                {
                    quit();
                }
            }

            // enter 
            enter(sp);

            return;
        }
    public:
        explicit
            SmartPtr(T* ptr = nullptr)
            : option(0)
        {
            // ptr <- new T(~~);
            if (ptr) {
                this->inner = new Inner();
                this->inner->ptr = ptr;
                this->inner->count = 1;
            }
        }
        explicit
            SmartPtr(T* ptr, const int option) // option 1,2,..
            : option(option)
        {
            // ptr <- new T(~~);
            if (ptr) {
                this->inner = new Inner();
                this->inner->ptr = ptr;
                this->inner->count = 1;
            }
        }
        SmartPtr(const SmartPtr<T>& sp)
            : option(sp.option)
        {
            initFromOther(sp);
        }
        virtual ~SmartPtr() /// virtual??
        {
            if (isOnlyOne())
            {
                remove(true);
            }
            else
            {
                remove(false);
            }
        }
    public:
        SmartPtr<T>& operator=(const SmartPtr<T>& _sp)
        {
            // temp link
            SmartPtr<T> tempLink(_sp);

            initFromOther(tempLink);

            return *this;
        }
        T& operator*()
        {
            // wizard::assertNotnullptr( ptr );
            return (*inner->ptr);
        }
        const T& operator*()const
        {
            // wizard::assertNotnullptr( ptr );
            return (*inner->ptr);
        }
    public:
        bool isOnlyOne()const
        {
            return this->inner && this->inner->count == 1;
        }
        bool isNULL()const
        {
            return nullptr == this->inner || nullptr == this->inner->ptr;
        }
        bool empty()const
        {
            return nullptr == this->inner;
        }
        /// remove return suceess?
        bool remove()
        {
            return remove(true);
        }
        bool remove(const bool bremove) // make private and delete =true, and make public remove() call remove( true ); - 2012.3.5 todo...
        {
            if (empty()) { return false; }
            if (!bremove && isOnlyOne()) { return false; } /// 2013.08.13 false means "no change"??
            if (this->inner->ptr && bremove)
            {
                delete this->inner->ptr; this->inner->ptr = nullptr;

                quit();
            }
            else
            {
                quit();
            }
            return true;
        }
    public:
        const T* operator->()const
        {
            //wizard::assertNotnullptr( ptr );
            return inner->ptr;
        }
        T* operator->()
        {
            //wizard::assertNotnullptr( ptr );
            return inner->ptr;
        }
        T* operator&()
        {
            return inner->ptr;
        }
        const T* operator&()const
        {
            return inner->ptr;
        }
        ///
    public:
        bool hasSameObject(const SmartPtr<T>& wsp) const
        {
            return this->inner == wsp.inner;
        }
    };
}

#endif // WIZSMARTPOINTER_H_INCLUDED
