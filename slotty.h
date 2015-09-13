#pragma once

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

#include <list>
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

    public:
        /**
         * \brief the default constructor
         */
        explicit slot() = default;

    private:
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
        ~slot()
        {
            auto& listeners = this->event_->listeners_;
            listeners.erase(std::remove(listeners.begin(), listeners.end(), this), listeners.end());
        }

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
         * listener is also invalidated. The slot filled is completely 
         * owned by the caller and must be delete'd by the caller.
         */
        void connect(
            callback_type callback, //!< [in] the callback function
            slot_type& slot //!< [in] the slot of fill
        ) const
        {
            slot.event_ = this;
            slot.callback_ = callback;
            this->listeners_.push_back(&slot);
        }

        /**
         * \brief raise the event
         *
         * This method dispatches a message to all listeners.
         */
        void raise(
            contexts... args //!< [in] the event arguments
        )
        {
            for (auto i = this->listeners_.begin(); i != this->listeners_.end(); ++i)
            {
                auto& f = *(*i);
                f.callback_(args...);
            }
        }

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
        mutable std::list<slot_type*> listeners_;

        friend class slot<contexts...>;
    };

    template <typename... contexts>
    class event_slot_pair
    {
    public:
        event<contexts...> event;
        slot<contexts...> slot;
    };

}