pcall(require, 'luacov')
local testcase = require('testcase')
local assert = require('assert')
local ref = require('ref')

function testcase.constants_exported()
    assert.is_number(ref.NOREF)
    assert.is_number(ref.REFNIL)
end

function testcase.get_returns_integer_reference()
    local k = ref.get('hello')
    assert.is_number(k)
    ref.del(k)
end

function testcase.val_returns_stored_value()
    local k = ref.get({
        key = 'val',
    })
    local v = ref.val(k)
    assert.is_table(v)
    assert.equal(v.key, 'val')
    ref.del(k)
end

function testcase.del_releases_reference_without_error()
    local k = ref.get('tmp')
    -- del without retval returns nothing; just ensure no error is raised
    assert.is_nil(ref.del(k))
end

function testcase.del_returns_value_when_asked()
    local k = ref.get('sentinel')
    local v = ref.del(k, true)
    assert.equal(v, 'sentinel')
end
