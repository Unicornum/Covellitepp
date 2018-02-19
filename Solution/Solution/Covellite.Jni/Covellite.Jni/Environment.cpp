
#include "stdafx.h"
#include "Environment.hpp"
#include <alicorn\std\exception.hpp>

using namespace covellite::jni;

/**
* \brief
*  Функция получения высоты статус-бара Android.
*
* \exception std::exception
*  - Действие невозможно (подробнее см. описание исключения).
*/
int32_t Environment::GetStatusBarHeight(void)
{
  auto ClassNativeActivity = m_pJniEnv->FindClass("android/app/NativeActivity");

  auto ClassWindow = m_pJniEnv->FindClass("android/view/Window");
  auto MethodGetWindow = m_pJniEnv->GetMethodID(ClassNativeActivity, 
    "getWindow", "()Landroid/view/Window;");
  auto lWindow = m_pJniEnv->CallObjectMethod(m_pActivity, MethodGetWindow);

  auto ClassView = m_pJniEnv->FindClass("android/view/View");
  auto MethodGetDecorView = m_pJniEnv->GetMethodID(ClassWindow, 
    "getDecorView", "()Landroid/view/View;");
  auto lDecorView = m_pJniEnv->CallObjectMethod(lWindow, MethodGetDecorView);

  auto ClassRect = m_pJniEnv->FindClass("android/graphics/Rect");
  auto ConstructorRect = m_pJniEnv->GetMethodID(ClassRect, "<init>", "()V");
  auto Rect = m_pJniEnv->NewObject(ClassRect, ConstructorRect);

  auto MethodGetWindowVisibleDisplayFrame = m_pJniEnv->GetMethodID(ClassView, 
    "getWindowVisibleDisplayFrame", "(Landroid/graphics/Rect;)V");
  m_pJniEnv->CallVoidMethod(lDecorView, MethodGetWindowVisibleDisplayFrame, Rect);

  auto fidTop = m_pJniEnv->GetFieldID(ClassRect, "top", "I");
  return m_pJniEnv->GetIntField(Rect, fidTop);
}

/**
* \brief
*  Функция для отображения клавиатуры.
* \details
*  - Понадобилась из-за того, что функция ANativeActivity_showSoftInput() не
*  работает.
*  - Скрытие клавиатуры можно не делать, т.к. Android скрывает ее при
*  нажатии кнопки "назад", но если нужно скрыть ее принудительно, можно 
*  использовать функцию ANativeActivity_hideSoftInput().
*
* \exception std::exception
*  - Действие невозможно (подробнее см. описание исключения).
*/
void Environment::ShowSoftKeyboard(void)
{
  auto ClassNativeActivity = m_pJniEnv->GetObjectClass(m_pActivity);

  // Retrieves Context.INPUT_METHOD_SERVICE.
  auto ClassContext = m_pJniEnv->FindClass("android/content/Context");
  auto FieldINPUT_METHOD_SERVICE = m_pJniEnv->GetStaticFieldID(ClassContext,
    "INPUT_METHOD_SERVICE", "Ljava/lang/String;");
  auto INPUT_METHOD_SERVICE = m_pJniEnv->GetStaticObjectField(ClassContext,
    FieldINPUT_METHOD_SERVICE);

  // Runs getSystemService(Context.INPUT_METHOD_SERVICE).
  auto ClassInputMethodManager = m_pJniEnv->FindClass(
    "android/view/inputmethod/InputMethodManager");
  auto MethodGetSystemService = m_pJniEnv->GetMethodID(ClassNativeActivity,
    "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
  auto lInputMethodManager = m_pJniEnv->CallObjectMethod(m_pActivity,
    MethodGetSystemService, INPUT_METHOD_SERVICE);

  // Runs getWindow().getDecorView().
  auto MethodGetWindow = m_pJniEnv->GetMethodID(ClassNativeActivity,
    "getWindow", "()Landroid/view/Window;");
  auto lWindow = m_pJniEnv->CallObjectMethod(m_pActivity, MethodGetWindow);
  auto ClassWindow = m_pJniEnv->FindClass("android/view/Window");
  auto MethodGetDecorView = m_pJniEnv->GetMethodID(
    ClassWindow, "getDecorView", "()Landroid/view/View;");
  auto lDecorView = m_pJniEnv->CallObjectMethod(lWindow, MethodGetDecorView);

  jint lFlags = 0;

  // Runs lInputMethodManager.showSoftInput(...).
  auto MethodShowSoftInput = m_pJniEnv->GetMethodID(ClassInputMethodManager, 
    "showSoftInput", "(Landroid/view/View;I)Z");
  m_pJniEnv->CallBooleanMethod(lInputMethodManager, MethodShowSoftInput,
    lDecorView, lFlags);
}
