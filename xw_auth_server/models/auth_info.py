# -*- coding: utf-8 -*-

from odoo import models, fields, api
import xw_cbase


class XwAuthServerAuthInfo(models.Model):
    '''
    Auth server auth info
    '''
    _name = 'xw_auth_server.auth_info'
    _description = 'Auth info'

    customer = fields.Many2one(
        string='Customer', 
        comodel_name='res.partner', 
        required=True)
    
    customer_name = fields.Char(
        string='Customer Name', 
        related='customer.name')

    machine_code = fields.Char(string='Machine code', required=True)
    user_count = fields.Integer(string='User Count', default=0, required=True)

    def _get_default_expire_date(self):
        """
        get default expire date, 2050-12-31
        """
        return fields.Date.from_string('2050-12-31')
    
    expire_date = fields.Date(
        string='Expire date', default=_get_default_expire_date, required=True)
    module_name = fields.Char(
        string='Module Name', required=True)

    reg_code = fields.Text(string='Reg Code')
    remark = fields.Text(string='Remark')

    def gen_reg_code(self):
        '''
        Generate reg code
        '''
        # format expire date to yyyy-mm-dd
        expire_date = fields.Date.to_string(self.expire_date)
        self.reg_code = xw_cbase.gen_reg_code(
            self.machine_code, 
            self.module_name, 
            self.customer_name, 
            self.user_count, 
            expire_date)
        
    def decode_reg_code(self):
        '''
        Test decode decode_machine_code
        '''
        code = str(self.reg_code)
        machine_code = xw_cbase.decode_machine_code(code)
        if self.remark is None:
            self.remark = machine_code
        else:
            self.remark = str(self.remark) + '\n' + machine_code