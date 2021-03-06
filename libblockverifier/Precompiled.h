/*
 * @CopyRight:
 * FISCO-BCOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FISCO-BCOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FISCO-BCOS.  If not, see <http://www.gnu.org/licenses/>
 * (c) 2016-2018 fisco-dev contributors.
 */
/** @file Precompiled.h
 *  @author mingzhenliu
 *  @date 20180921
 */
#pragma once

#include <libdevcore/Address.h>
#include <map>
#include <memory>

namespace dev
{
namespace storage
{
class Table;
}  // namespace storage

namespace blockverifier
{
class ExecutiveContext;
class Precompiled : public std::enable_shared_from_this<Precompiled>
{
public:
    typedef std::shared_ptr<Precompiled> Ptr;

    virtual ~Precompiled(){};

    virtual std::string toString() { return ""; }

    virtual bytes call(std::shared_ptr<dev::blockverifier::ExecutiveContext> _context,
        bytesConstRef _param, Address const& _origin = Address()) = 0;

    virtual uint32_t getParamFunc(bytesConstRef _param)
    {
        auto funcBytes = _param.cropped(0, 4);
        uint32_t func = *((uint32_t*)(funcBytes.data()));

        return ((func & 0x000000FF) << 24) | ((func & 0x0000FF00) << 8) |
               ((func & 0x00FF0000) >> 8) | ((func & 0xFF000000) >> 24);
    }

    virtual uint32_t getFuncSelector(std::string const& _functionName);
    virtual bytesConstRef getParamData(bytesConstRef _param) { return _param.cropped(4); }

protected:
    std::map<std::string, uint32_t> name2Selector;
    std::shared_ptr<dev::storage::Table> openTable(
        std::shared_ptr<dev::blockverifier::ExecutiveContext> _context,
        const std::string& _tableName);
    std::shared_ptr<dev::storage::Table> createTable(
        std::shared_ptr<dev::blockverifier::ExecutiveContext> _context,
        const std::string& _tableName, const std::string& _keyField, const std::string& _valueField,
        Address const& origin);
};

}  // namespace blockverifier

}  // namespace dev
