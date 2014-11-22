#pragma once
#ifndef __slotty_h__
#define __slotty_h__

/*
 * slotty.h
 *
 *  Created on: Oct 23, 2014
 *      Author: Pranjal Raihan
 */

/******************************************************************************
 * Copyright 2014 See AUTHORS file.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include <vector>
#include <algorithm>
#include <functional>

namespace slotty
{
    template <typename... contexts>
    class event;

    template <typename... contexts>
    class slot final
    {
    public:
        /**
         * \typedef the callback function type
         */
        typedef std::function<void(contexts...)> callback_type;

    private:
        /**
         * \brief the default constructor
         */
        explicit slot() = default;

        /**
         * \brief no copy
         */
        explicit slot(const slot<contexts...>&) = delete;

        /**
         * \brief no assign
         */
        slot<contexts...>& operator=(const slot<contexts...>&) = delete;

    public:
        /**
         * \brief the non-virtual destructor
         */
        ~slot();

    private:
        /**
         * \brief pointer to associated event
         */
        const event<contexts...>* event_;

        /**
         * \brief the callback function
         */
        callback_type callback_;

        friend class event<contexts...>;
    };

    template <typename... contexts>
    class event final
    {
    public:
        /**
         * \typedef the associated slot type
         */
        typedef typename slot<contexts...> slot_type;
        /**
         * \typedef the callback function type
         */
        typedef typename slot_type::callback_type callback_type;

        /**
         * \brief the default constructor
         */
        explicit event() = default;

        /**
        * \brief the non-virtual destructor
        */
        ~event() = default;

        /**
         * \brief add a listener of to the list of listeners
         *
         * The lifetime of the listener is tied to the slot returned. 
         * When the slot is deleted (destructor is called), the 
         * listener is also invalidated. The slot returned is completely 
         * owned by the caller and must be delete'd by the user.
         *
         * \return the slot object for listener lifetime control
         */
        slot_type* connect(
            callback_type callback //!< [in] the callback function
        ) const;

        /**
         * \brief raise the event
         *
         * This method dispatches a message to all listeners.
         */
        void raise(
            contexts... args //!< [in] the event arguments
        );

    private:
        /**
         * \brief no copy
         */
        explicit event(const event<contexts...>&) = delete;

        /**
         * \brief no assign
         */
        event<contexts...>& operator=(const event<contexts...>&) = delete;

    private:
        /**
         * \brief list of listeners
         */
        mutable std::vector<slot_type*> listeners_;

        friend class slot<contexts...>;
    };




    // template implementation

    template <typename... contexts>
    typename event<contexts...>::slot_type* event<contexts...>::connect(
        typename event<contexts...>::callback_type callback
    ) const
    {
        auto s = new typename event<contexts...>::slot_type();
        s->event_ = this;
        s->callback_ = callback;
        this->listeners_.push_back(s);
        return s;
    }

    template <typename... contexts>
    void event<contexts...>::raise(contexts... args)
    {
        for (auto i = this->listeners_.begin(); i != this->listeners_.end(); ++i)
        {
            auto& f = *(*i);
            f.callback_(args...);
        }
    }

    template <typename... contexts>
    slot<contexts...>::~slot()
    {
        auto& listeners = this->event_->listeners_;
        listeners.erase(std::remove(listeners.begin(), listeners.end(), this), listeners.end());
    }

}

#endif